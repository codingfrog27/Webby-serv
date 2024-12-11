#include "CGI.hpp"
// #include <wait.h>
#include <sys/wait.h>

CGI::CGI(int *fdIn, int *fdOut, int *fdError) : _fdIn(fdIn), _fdOut(fdOut), _fdError(fdError){
	return ;
}

CGI::~CGI(){
	for (char* str : _envp){
		delete[] str;
	}
}

//needs refactoring
std::string	CGI::invokeCGI(Request* request, Response* response){
	int PID = fork();
	if (PID == -1){
		closePipes();
		response->autoFillResponse("500 Internal Server Error: fork");
		return response->generateResponse();
	}
	if (PID == 0){ //child
		dup2(_fdIn[0], STDIN_FILENO);
		dup2(_fdOut[1], STDOUT_FILENO);
		dup2(_fdError[1], STDERR_FILENO);
		closePipes();
		int status = 0;
		response = CGI::executeScript(request, response); // into buffer and then to response or write errors to stderr(file)
		//if something went wrong adjust status
		exit(status);
	}
	else { //parent
		close(_fdIn[0]);
		close(_fdOut[1]);
		close(_fdError[1]);
		if (request->_method_type == POST)
			write(_fdIn[1], request->getBody().data(), request->getBody().size());
		close(_fdIn[1]);
		char buffer[BUFFER_SIZE];
		int bytesRead = 0;
		std::string responseBuffer;
		while ((bytesRead = read(_fdOut[0], buffer, BUFFER_SIZE)) > 0){
			// response->setBody(std::vector<char>(buffer, buffer + bytesRead));
			// response->setBody(std::string(buffer, bytesRead));
			responseBuffer.append(buffer, bytesRead);
		}
		close(_fdOut[0]);
		if (bytesRead == -1){
			response->autoFillResponse("500 Internal Server Error: read");
			close(_fdError[0]);
			return response->generateResponse();
		}
		bytesRead = 0;
		std::string error;
		while ((bytesRead = read(_fdError[0], buffer, BUFFER_SIZE)) > 0){
			error.append(buffer, bytesRead);
		}
		close(_fdError[0]);
		if (bytesRead == -1){
			response->autoFillResponse("500 Internal Server Error: read");
			return response->generateResponse();
		}
		if (!error.empty()){
			response->autoFillResponse("500 Internal Server Error: script: " + error);
			return response->generateResponse();
		}
		if (waitpid(PID, NULL, WNOHANG) == -1){
			response->autoFillResponse("500 Internal Server Error: waitpid");
			return response->generateResponse();
		}
		return request->_http_version + " 200 OK\r\n" + responseBuffer;
	}
	return response->generateResponse();
}

Response*	CGI::executeScript(Request* request, Response* response){
	// char* argv[] = {strdup(request->_filePath.c_str()), NULL};
	char* argv[] = {strdup(request->_filePath.substr(request->_filePath.rfind("/") + 1).c_str()), NULL};
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
