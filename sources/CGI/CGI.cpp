#include "CGI.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include <cstring>
#include <sys/wait.h>



/*	Constructors & destructors	*/
CGI::CGI(int *fdIn, int *fdOut, int *fdError) : _fdIn(fdIn), _fdOut(fdOut), _fdError(fdError){
	_CGIHandlerStatus = CGIHandlerStatus::NOT_STARTED;
	_bytesWrittenToChild = 0;
	_scriptError = "";
	return ;
}

CGI::~CGI(){
	// for (char* str : _envp){
	// 	delete[] str;
	// }
	// delete this;
	return ;
}

/*	Member functions	*/
//needs refactoring
void	CGI::invokeCGI(Request* request, Response* response){
	std::cout << MAGENTA "~ CGI Invoked ~" << std::endl;
	response->setResponseHandlerStatus(responseHandlerStatus::IN_CGI);
	if (_CGIHandlerStatus == CGIHandlerStatus::NOT_STARTED){
		_CGIHandlerStatus = CGIHandlerStatus::IN_PROGRESS;
		_PID = fork();
		if (_PID == -1){
			closePipes();
			response->autoFillResponse("500 Internal Server Error: fork");
			return ;
		}
		if (_PID == 0){ //child
			dup2(_fdIn[0], STDIN_FILENO);
			dup2(_fdOut[1], STDOUT_FILENO);
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
	}
	if (_PID != 0) { //parent
		if (_CGIHandlerStatus == CGIHandlerStatus::IN_PROGRESS && request->_method_type == Http_method::GET)
			_CGIHandlerStatus = CGIHandlerStatus::WAITING_FOR_CHILD;
		if ((_CGIHandlerStatus == CGIHandlerStatus::IN_PROGRESS || _CGIHandlerStatus == CGIHandlerStatus::WRITING_TO_CHILD) && request->_method_type == Http_method::POST){
			_CGIHandlerStatus = CGIHandlerStatus::WRITING_TO_CHILD;
			// std::cout << MAGENTA "Req Body	: " << request->getBody() << std::endl;
			int bytes = write(_fdIn[1], request->getBody().data() + _bytesWrittenToChild, BUFFER_SIZE);
			if (bytes == -1){
				response->autoFillResponse("500 Internal Server Error: write");
				close(_fdIn[1]);
				close(_fdOut[0]);
				close(_fdError[0]);
				return ;
			}
			_bytesWrittenToChild += bytes;
			if (_bytesWrittenToChild == request->getBody().size()){
				close(_fdIn[1]);
				_CGIHandlerStatus = CGIHandlerStatus::WAITING_FOR_CHILD;
			}
			return ;
		}
		if (_CGIHandlerStatus == CGIHandlerStatus::WAITING_FOR_CHILD){
			int status = 0;
			if (waitpid(_PID, &status, WNOHANG) == -1){
				response->autoFillResponse("500 Internal Server Error: waitpid");
				close(_fdOut[0]);
				close(_fdError[0]);
				return ;
			}
			if (WIFEXITED(status)){
				_CGIHandlerStatus = CGIHandlerStatus::READING_FDOUT;
				// response->setResponseBuffer(request->_http_version + " 200 OK\r\n"); needed if generate response doesnt work!
			}
			return ;
		}
		if (_CGIHandlerStatus == CGIHandlerStatus::READING_FDOUT){
			char buffer[BUFFER_SIZE];
			int bytes = read(_fdOut[0], buffer, BUFFER_SIZE);
			if (bytes == -1){
				response->autoFillResponse("500 Internal Server Error: read");
				close(_fdOut[0]);
				close(_fdError[0]);
				return ;
			}
			response->setBody(std::string(buffer, bytes)); //setResponseBuffer if setBody is not working
			if (bytes == 0){
				close(_fdOut[0]);
				_CGIHandlerStatus = CGIHandlerStatus::READING_FDERROR;
			}
			return ;
		}
		// std::cout << MAGENTA "Res Body	: " << responseBuffer << std::endl;
		if (_CGIHandlerStatus == CGIHandlerStatus::READING_FDERROR){
			char buffer[BUFFER_SIZE];
			int bytes = read(_fdError[0], buffer, BUFFER_SIZE);
			if (bytes == -1){
				response->autoFillResponse("500 Internal Server Error: read");
				close(_fdError[0]);
				return ;
			}
			_scriptError.append(buffer, bytes);
			if (bytes == 0){
				close(_fdError[0]);
				if (!_scriptError.empty()){
					response->autoFillResponse("500 Internal Server Error: script: " + _scriptError);
					return ;
				}
				_CGIHandlerStatus = CGIHandlerStatus::FINISHED;
			}
			return ;
		}
	}
	if (_CGIHandlerStatus == CGIHandlerStatus::FINISHED){
		response->setStatus("200 OK");
		response->setResponseBuffer(response->generateResponse());
		response->setResponseHandlerStatus(responseHandlerStatus::READY_TO_WRITE);
	}
	return ;
}

void	CGI::executeScript(Request* request, Response* response){
	// char* argv[] = {strdup(request->_filePath.c_str()), NULL};
	std::string arg = request->_filePath.substr(request->_filePath.rfind("/") + 1);
	char* argv[] = {const_cast<char *>(arg.c_str()), NULL};
	if (execve(request->_filePath.c_str(), argv, _envp.data()) == -1)
		response->autoFillResponse("500 Internal Server Error: execve : " + std::string(strerror(errno)));
	return ;
}

void	CGI::closePipes(){
	for (int i = 0; i < 2; i++){
		close(_fdIn[i]);
		close(_fdOut[i]);
		close(_fdError[i]);
	}
	return ;
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
