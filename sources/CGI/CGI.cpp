#include "CGI.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Connection.hpp"
#include <cstring>
#include <sys/wait.h>



/*	Constructors & destructors	*/

// CGI::CGI(int *fdIn, int *fdOut, int *fdError) : _fdIn(fdIn), _fdOut(fdOut), _fdError(fdError){
// 	_CGIHandlerStatus = CGIHandlerStatus::NOT_STARTED;
// 	// _bytesWrittenToChild = 0;
// 	// _scriptError = "";
// 	_pollFdIn.fd = _fdIn[1];
// 	_pollFdIn.events = POLLOUT;
// 	_pollFdOut.fd = _fdOut[0];
// 	_pollFdOut.events = POLLIN;
// 	_pollFdError.fd = _fdError[0];
// 	_pollFdError.events = POLLIN;
// 	return ;
// }

CGI::CGI(Connection* connection, std::vector<pollfd>* CGIPollFDs) {
	_CGIHandlerStatus = CGIHandlerStatus::NOT_STARTED;

	if (pipe(_fdIn) == -1) {
		connection->_response.autoFillResponse("500 Internal Server Error: pipe fdIn");
		return ;
	}
	if (pipe(_fdOut) == -1) {
		closePipes();
		connection->_response.autoFillResponse("500 Internal Server Error: pipe fdOut");
		return ;
	}
	if (pipe(_fdError) == -1) {
		closePipes();
		connection->_response.autoFillResponse("500 Internal Server Error: pipe fdError");
		return ;
	}
	CGIPollFDs->emplace_back(pollfd{_fdIn[1], POLLOUT, 0});
	CGIPollFDs->emplace_back(pollfd{_fdOut[0], POLLIN, 0});
	CGIPollFDs->emplace_back(pollfd{_fdError[0], POLLIN, 0});
	setupCGIEnvironment(&connection->_request);
	return ;
}

CGI::~CGI(){
	for (char* str : _envp)
		free(str);
	return ;
}

/*	Member functions	*/
//needs refactoring to adjust connectstatus
void	CGI::invokeCGI(Request* request, Response* response){
	// std::cout << MAGENTA "~ CGI Invoked ~" << std::endl;
	// response->setResponseHandlerStatus(responseHandlerStatus::IN_CGI);
	// if (_CGIHandlerStatus == CGIHandlerStatus::NOT_STARTED){
	// 	_CGIHandlerStatus = CGIHandlerStatus::IN_PROGRESS;
	_PID = fork();
	if (_PID == -1){
		closePipes();
		response->autoFillResponse("500 Internal Server Error: fork");
		_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
		return ;	// }
	}
	if (_PID == 0){ //child
		dup2(_fdError[1], STDERR_FILENO);
		closePipes();
		int status = 0;
		CGI::executeScript(request, response);
		exit(status); //might not be needed
	}
	else { //parent
		close(_fdIn[0]);
		close(_fdOut[1]);
		close(_fdError[1]);
	}
	return ;
	// }
	// if (_PID > 0) { //parent
	// 	if (_CGIHandlerStatus == CGIHandlerStatus::IN_PROGRESS && request->_method_type == GET)
	// 		_CGIHandlerStatus = CGIHandlerStatus::WAITING_FOR_CHILD;
	// 	if ((_CGIHandlerStatus == CGIHandlerStatus::IN_PROGRESS || _CGIHandlerStatus == CGIHandlerStatus::WRITING_TO_CHILD) && request->_method_type == POST){
	// 		_CGIHandlerStatus = CGIHandlerStatus::WRITING_TO_CHILD;
	// 		std::cout << MAGENTA "Writing to child" RESET << std::endl;
	// 		// std::cout << MAGENTA "Req Body	: " << request->getBody() << std::endl;
	// 		size_t bodySize = request->getBody().size();
	// 		size_t bytesWritten = 0;
	// 		while (bytesWritten < bodySize){
	// 			size_t bytesToWrite = std::min((size_t)BUFFER_SIZE, bodySize - bytesWritten);
	// 			size_t bytes = write(_fdIn[1], request->getBody().data() + bytesWritten, bytesToWrite);
	// 			if (bytes < 0){
	// 				response->autoFillResponse("500 Internal Server Error: write - to child");
	// 				closePipes();
	// 				return ;
	// 			}
	// 			bytesWritten += bytes;
	// 		}
	// 		if (bytesWritten >= bodySize){
	// 			close(_fdIn[1]);
	// 			_CGIHandlerStatus = CGIHandlerStatus::WAITING_FOR_CHILD;
	// 		}
	// 		// return ;
	// 	}
	// 	if (_CGIHandlerStatus == CGIHandlerStatus::WAITING_FOR_CHILD){
	// 		std::cout << MAGENTA "Waiting for child" RESET << std::endl;
	// 		int status = 0;
	// 		if (waitpid(_PID, &status, WNOHANG) == -1){
	// 			response->autoFillResponse("500 Internal Server Error: waitpid");
	// 			close(_fdOut[0]);
	// 			close(_fdError[0]);
	// 			return ;
	// 		}
	// 		if (WIFEXITED(status)){
	// 			_CGIHandlerStatus = CGIHandlerStatus::READING_FDOUT;
	// 			// response->setResponseBuffer(request->_http_version + " 200 OK\r\n"); //needed if generate response doesnt work!
	// 		}
	// 		return ;
	// 	}
	// 	if (_CGIHandlerStatus == CGIHandlerStatus::READING_FDOUT){
	// 		std::cout << MAGENTA "Reading from child" RESET << std::endl;
	// 		char buffer[BUFFER_SIZE];
	// 		while (1){
	// 			int bytes = read(_fdOut[0], buffer, BUFFER_SIZE);
	// 			if (bytes == -1){
	// 				response->autoFillResponse("500 Internal Server Error: read");
	// 				closePipes();
	// 				return ;
	// 			}
	// 			response->setResponseBuffer(std::string(buffer, bytes)); //setResponseBuffer if setBody is not working
	// 			if (bytes == 0){
	// 				close(_fdOut[0]);
	// 				_CGIHandlerStatus = CGIHandlerStatus::READING_FDERROR;
	// 				break ;
	// 			}
	// 		}
	// 		// return ;
	// 	}
	// 	// std::cout << MAGENTA "Res Body	: " << responseBuffer << std::endl;
	// 	if (_CGIHandlerStatus == CGIHandlerStatus::READING_FDERROR){
	// 		std::cout << MAGENTA "Reading from child error" RESET << std::endl;
	// 		char buffer[BUFFER_SIZE];
	// 		while (1){
	// 			int bytes = read(_fdError[0], buffer, BUFFER_SIZE);
	// 			if (bytes == -1){
	// 				response->autoFillResponse("500 Internal Server Error: read");
	// 				closePipes();
	// 				return ;
	// 			}
	// 			_scriptError.append(buffer, bytes);
	// 			if (bytes == 0){
	// 				close(_fdError[0]);
	// 				if (!_scriptError.empty()){
	// 					response->autoFillResponse("500 Internal Server Error: script: " + _scriptError);
	// 					return ;
	// 				}
	// 				_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
	// 				break;
	// 			}
	// 		}
	// 		// return ;
	// 	}
	// }
	// if (_CGIHandlerStatus == CGIHandlerStatus::FINISHED){
	// 	std::cout << MAGENTA "Ready to write\n" RESET << response->getResponseBuffer() << std::endl;
	// 	// response->setStatus("200 OK");
	// 	// response->setResponseBuffer(response->generateResponse());
	// 	response->setResponseHandlerStatus(responseHandlerStatus::READY_TO_WRITE);
	// }
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
		response->autoFillResponse("500 Internal Server Error: write");
		closePipes();
		_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
		return ;
	}
	_bytesWrittenToChild += bytes;
	if (_bytesWrittenToChild == request->getBody().size()){
		// End of file
		close(_fdIn[1]);
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
	} 
	else if (bytes == 0) {
		// End of file
		close(_fdOut[0]);
		_CGIHandlerStatus = CGIHandlerStatus::READING_FDERROR;
	} 
	else {
		// Handle error
		response->autoFillResponse("500 Internal Server Error: read");
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
	} 
	else if (bytes == 0) {
		// End of file
		close(_fdError[0]);
		if (!_scriptError.empty()) {
			response->autoFillResponse("500 Internal Server Error: script: " + _scriptError);
		}
		_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
	} 
	else {
		// Handle error
		response->autoFillResponse("500 Internal Server Error: read");
		closePipes();
		_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
	}
	return ;
}

bool	CGI::childIsRunning(Response* response){
	int status = 0;
	pid_t result = waitpid(_PID, &status, WNOHANG);
	if (result == -1) {
		response->autoFillResponse("500 Internal Server Error: waitpid");
		closePipes();
		_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
		return false;
	} 
	else if (result == 0) {
		// Child is still running
		return true;
	} 
	else if (WIFEXITED(status)) {
		_CGIHandlerStatus = CGIHandlerStatus::CHILD_IS_FINISHED;
		return false;
	}
}

void	CGI::executeScript(Request* request, Response* response){
	// char* argv[] = {strdup(request->_filePath.c_str()), NULL};
	std::string arg = request->_filePath.substr(request->_filePath.rfind("/") + 1);
	char* argv[] = {const_cast<char *>(arg.c_str()), NULL};
	if (execve(request->_filePath.c_str(), argv, _envp.data()) == -1)
		response->autoFillResponse("500 Internal Server Error: execve : " + std::string(strerror(errno)));
	return ;
}

void CGI::closePipes(){
	for (int i = 0; i < 2; i++){
		if (_fdIn[i] != -1) close(_fdIn[i]);
		if (_fdOut[i] != -1) close(_fdOut[i]);
		if (_fdError[i] != -1) close(_fdError[i]);
	}
}

void CGI::setupCGIEnvironment(Request* request) {
	if (request->_method_type == GET) {
		CGI::addToEnvp("REQUEST_METHOD", "GET");
		if (request->_URI.find("?") != std::string::npos) {
			CGI::addToEnvp("QUERY_STRING", request->_URI.substr(request->_URI.find("?") + 1));
			std::cout << LILAC "QUERY_STRING: " << request->_URI.substr(request->_URI.find("?") + 1) << RESET << std::endl;
		}
	}
	else if (request->_method_type == POST) {
		CGI::addToEnvp("REQUEST_METHOD", "POST");
		CGI::addToEnvp("CONTENT_TYPE", request->_headers["Content-Type"]);
		CGI::addToEnvp("CONTENT_LENGTH", request->_headers["Content-Length"]);
	}
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

int	CGI::getFdIn(void){
	return _fdIn[1];
}

int	CGI::getFdOut(void){
	return _fdOut[0];
}

int	CGI::getFdError(void){
	return _fdError[0];
}
