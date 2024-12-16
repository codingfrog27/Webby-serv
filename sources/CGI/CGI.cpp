#include "CGI.hpp"
// #include <wait.h>
#include <sys/wait.h>

CGI::CGI(int *fdIn, int *fdOut, int *fdError) : _fdIn(fdIn), _fdOut(fdOut), _fdError(fdError){
	return ;
}

CGI::~CGI(){
	// for (char* str : _envp){
	// 	delete[] str;
	// }
	// delete this;
	return ;
}

//needs refactoring
void	CGI::invokeCGI(Request* request, Response* response){
	std::cout << MAGENTA "~ CGI Invoked ~" << std::endl;
	int PID = fork();
	if (PID == -1){
		closePipes();
		response->autoFillResponse("500 Internal Server Error: fork");
		return ;
	}
	if (PID == 0){ //child
		dup2(_fdIn[0], STDIN_FILENO);
		dup2(_fdOut[1], STDOUT_FILENO);
		dup2(_fdError[1], STDERR_FILENO);
		closePipes();
		int status = 0;
		response = CGI::executeScript(request, response);
		//if something went wrong adjust status
		exit(status);
	}
	else { //parent
		close(_fdIn[0]);
		close(_fdOut[1]);
		close(_fdError[1]);
		if (request->_method_type == POST){
			std::cout << MAGENTA "Req Body	: " << request->getBody() << std::endl;
			write(_fdIn[1], request->getBody().data(), request->getBody().size());
		}
		close(_fdIn[1]);
		char buffer[BUFFER_SIZE];
		int bytesRead = 0;
		while ((bytesRead = read(_fdOut[0], buffer, BUFFER_SIZE)) > 0)
			response->setResponseBuffer(std::string(buffer, bytesRead));
		close(_fdOut[0]);
		std::cout << MAGENTA "Res Body	: " << responseBuffer << std::endl;
		if (bytesRead == -1){
			response->autoFillResponse("500 Internal Server Error: read");
			close(_fdError[0]);
			return ;
		}
		bytesRead = 0;
		std::string error;
		while ((bytesRead = read(_fdError[0], buffer, BUFFER_SIZE)) > 0){
			error.append(buffer, bytesRead);
		}
		close(_fdError[0]);
		std::cout << MAGENTA "Err Body	: " << error << std::endl;
		if (bytesRead == -1){
			response->autoFillResponse("500 Internal Server Error: read");
			return ;
		}
		if (!error.empty()){
			response->autoFillResponse("500 Internal Server Error: script: " + error);
			return ;
		}
		if (waitpid(PID, NULL, WNOHANG) == -1){
			response->autoFillResponse("500 Internal Server Error: waitpid");
			return ;
		}
		return request->_http_version + " 200 OK\r\n" + responseBuffer;
	}
	response->setResponseBuffer(response->generateResponse());
	return ;
}

Response*	CGI::executeScript(Request* request, Response* response){
	// char* argv[] = {strdup(request->_filePath.c_str()), NULL};
	std::string arg = request->_filePath.substr(request->_filePath.rfind("/") + 1);
	char* argv[] = {const_cast<char *>(arg.c_str()), NULL};
	if (execve(request->_filePath.c_str(), argv, _envp.data()) == -1)
		response->autoFillResponse("500 Internal Server Error: execve : " + std::string(strerror(errno)));
	return response;
}

void CGI::setupCGIEnvironment(Request* request) {
	if (request->_method_type == GET) {
		CGI::addToEnvp("REQUEST_METHOD", "GET");
		if (request->_URI.find("?") != std::string::npos) {
			CGI::addToEnvp("QUERY_STRING", request->_URI.substr(request->_URI.find("?") + 1));
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

void	CGI::closePipes(){
	for (int i = 0; i < 2; i++){
		close(_fdIn[i]);
		close(_fdOut[i]);
		close(_fdError[i]);
	}
	return ;
}
