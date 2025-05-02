#include "CGI.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Connection.hpp"
#include <cstring>
#include <sys/wait.h>

/*	Constructors & destructors	*/

CGI::CGI(Connection* connection, std::vector<pollfd> &CGIPollFDs) : _clientFD(connection->_clientFD){
	_CGIHandlerStatus = CGIHandlerStatus::NOT_STARTED;
	_bytesWrittenToChild = 0;
	_PID = -1;

	if (pipe(_fdIn) == -1) {
		connection->_response.autoFillResponse("500 Internal Server Error: pipe fdIn", "", "");
		return ;
	}
	if (pipe(_fdOut) == -1) {
		closePipes();
		connection->_response.autoFillResponse("500 Internal Server Error: pipe fdOut", "", "");
		return ;
	}
	if (pipe(_fdError) == -1) {
		closePipes();
		connection->_response.autoFillResponse("500 Internal Server Error: pipe fdError", "", "");
		return ;
	}
	if (connection->_request._method_type == Http_method::POST){
		CGIPollFDs.emplace_back(pollfd{_fdIn[1], POLLOUT, 0});
	}
	else {
		close(_fdIn[1]);
	}
	CGIPollFDs.emplace_back(pollfd{_fdOut[0], POLLIN, 0});
	CGIPollFDs.emplace_back(pollfd{_fdError[0], POLLIN, 0});
	setupCGIEnvironment(&connection->_request);
	_maxDuration = setTimeout(connection->_config->getTimeout()/1000);
	return ;
}

CGI::~CGI(){
	for (char* str : _envp)
		free(str);
	return ;
}

/*	Member functions	*/
void	CGI::invokeCGI(Request* request, Response* response){
	_startTime = getStartTime();
	_PID = fork();
	if (_PID == -1){
		closePipes();
		response->autoFillResponse("500 Internal Server Error: fork", "", "");
		_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
		return ;
	}
	if (_PID == 0){ //child
		if (request->_method_type == Http_method::POST){
			dup2(_fdIn[0], STDIN_FILENO);
		}
		dup2(_fdOut[1], STDOUT_FILENO);
		dup2(_fdError[1], STDERR_FILENO);
		closePipes();
		CGI::executeScript(request, response);
	}
	else { //parent
		close(_fdIn[0]);
		close(_fdOut[1]);
		close(_fdError[1]);
		_CGIHandlerStatus = CGIHandlerStatus::IN_PROGRESS;
	}
	return ;
}

void CGI::writeToCGI(Request* request, Response* response) {
	_CGIHandlerStatus = CGIHandlerStatus::WRITING_TO_CHILD;
	size_t n = request->getBody().size() - _bytesWrittenToChild;
	if (n > BUFFER_SIZE)
		n = BUFFER_SIZE;
	int bytes = write(_fdIn[1], request->getBody().data() + _bytesWrittenToChild, n);
	if (bytes == -1){
		// Handle error
		response->autoFillResponse("500 Internal Server Error: write", "", "");
		closePipes();
		_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
		return ;
	}
	_bytesWrittenToChild += bytes;
	if (_bytesWrittenToChild == request->getBody().size()){
		// End of file
		close(_fdIn[1]);
		_CGIHandlerStatus = CGIHandlerStatus::WAITING_FOR_CHILD;
	}
	return ;
}

void CGI::readFromCGI(Response* response) {
	_CGIHandlerStatus = CGIHandlerStatus::READING_FDOUT;
	char buffer[BUFFER_SIZE];
	int bytes = read(_fdOut[0], buffer, BUFFER_SIZE);
	if (bytes > 0) {
		// Append the data to the response buffer
		response->setResponseBuffer(std::string(buffer, bytes));
		if (bytes < BUFFER_SIZE) {
			// End of file
			close(_fdOut[0]);
			_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
		}
	}
	else if (bytes == 0) {
		// End of file
		close(_fdOut[0]);
		_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
	}
	else {
		// Handle error
		response->autoFillResponse("500 Internal Server Error: read", "", "");
		closePipes();
		_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
	}
	return ;
}

void CGI::readErrorFromCGI(Response* response) {
	_CGIHandlerStatus = CGIHandlerStatus::READING_FDERROR;
	char buffer[BUFFER_SIZE];
	int bytes = read(_fdError[0], buffer, BUFFER_SIZE);
	if (bytes > 0) {
		// Append the error data to the script error
		_scriptError.append(buffer, bytes);
		if (bytes < BUFFER_SIZE) {
			// End of file
			close(_fdError[0]);
			if (!_scriptError.empty()) {
				response->autoFillResponse("500 Internal Server Error: script", "", _scriptError);
			}
			_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
		}
	}
	else if (bytes == 0) {
		// End of file
		close(_fdError[0]);
		if (!_scriptError.empty()) {
			response->autoFillResponse("500 Internal Server Error: script", "", _scriptError);
		}
		_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
		response->setResponseHandlerStatus(responseHandlerStatus::READY_TO_WRITE);
	}
	else {
		// Handle error
		response->autoFillResponse("500 Internal Server Error: read", "", "");
		closePipes();
		_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
	}
	return ;
}

bool	CGI::CGIisTimedOut(void){
	if (isTimedOut(_startTime, _maxDuration)) {
		return true;
	}
	return false;
}

void CGI::killChild(void) {
	if (_PID != -1) {
		kill(_PID, SIGKILL);
		waitpid(_PID, NULL, 0); // Kill the zombies
		_PID = -1;
	}
	closePipes();
}

bool CGI::childIsRunning(Response* response) {
	int status = 0;
	pid_t result = waitpid(_PID, &status, WNOHANG);
	if (result == -1) {
		response->autoFillResponse("500 Internal Server Error: waitpid", "");
		closePipes();
		_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
		return false;
	} else if (WIFEXITED(status)) {
		if (WEXITSTATUS(status) != 0) {
			response->autoFillResponse("500 Internal Server Error: script exited with status " + std::to_string(WEXITSTATUS(status)), "");
		}
		_childIsRunningStatus = false;
		return false;
	}
	return true;
}

void	CGI::executeScript(Request* request, Response* response){
	std::string arg = request->_filePath.substr(request->_filePath.rfind("/") + 1);
	char* argv[] = {const_cast<char *>(arg.c_str()), NULL};
	if (execve(request->_filePath.c_str(), argv, _envp.data()) == -1){
		response->autoFillResponse("500 Internal Server Error: execve : " + std::string(strerror(errno)), "", "");
		exit(1);
	}
	return ;
}

void CGI::closePipes(){
	for (int i = 0; i < 2; i++){
		if (_fdIn[i] != -1) {
			close(_fdIn[i]);
			_fdIn[i] = -1;
		}
		if (_fdOut[i] != -1) {
			close(_fdOut[i]);
			_fdOut[i] = -1;
		}
		if (_fdError[i] != -1) {
			close(_fdError[i]);
			_fdError[i] = -1;
		}
		
	}
}

void CGI::setupCGIEnvironment(Request* request) {
	if (request->_method_type == Http_method::GET) {
		CGI::addToEnvp("REQUEST_METHOD", "GET");
		if (request->_URI.find("?") != std::string::npos) {
			CGI::addToEnvp("QUERY_STRING", request->_URI.substr(request->_URI.find("?") + 1));
		}
	}
	else if (request->_method_type == Http_method::POST) {
		CGI::addToEnvp("REQUEST_METHOD", "POST");
		CGI::addToEnvp("CONTENT_TYPE", request->_headers["Content-Type"]);
		CGI::addToEnvp("CONTENT_LENGTH", request->_headers["Content-Length"]);
	}
	CGI::addToEnvp("STATUS_CODE", std::to_string(request->_statusCode).c_str());
	CGI::addToEnvp("SCRIPT_FILENAME", request->_filePath);
	CGI::addToEnvp("SERVER_PROTOCOL", "HTTP/1.1");
	CGI::addToEnvp("SERVER_NAME", request->_headers["Host"]);
	_envp.push_back(nullptr);
	return ;
}

void CGI::addToEnvp(std::string key, std::string value){
	std::string temp = key + "=" + value;
	_envp.push_back(strdup(temp.c_str()));
	return ;
}


/*	Setters & getters	*/
void	CGI::setCGIHandlerStatus(CGIHandlerStatus status){
	_CGIHandlerStatus = status;
	return ;
}

CGIHandlerStatus	CGI::getCGIHandlerStatus() const{
	return _CGIHandlerStatus;
}

bool	CGI::getChildIsRunningStatus(void){
	return _childIsRunningStatus;
}

int	CGI::getFdIn(void){
	return _fdIn[1];
}

int	CGI::getFdOut(void){
	return _fdOut[0];
}

int	CGI::getFdError(void){
	return _fdError[0];
}

void CGI::closeFdIn(void){
	close(_fdIn[1]);
	_fdIn[1] = -1;
}

void CGI::closeFdOut(void){
	close(_fdOut[0]);
	_fdOut[0] = -1;
}

void CGI::closeFdError(void){
	close(_fdError[0]);
	_fdError[0] = -1;
}

int	CGI::getClientFD(void){
	return _clientFD;
}
