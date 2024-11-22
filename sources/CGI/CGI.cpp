#include "CGI.hpp"

CGI::CGI(int fd0, int fd1) : _fd0(fd0), _fd1(fd1){
	return ;
}

CGI::~CGI(){
	for (char* str : _envp){
		delete[] str;
	}
}

//IS THIS POSSIBLE?

// Response*	CGI::invokeCGI(Request* request, Response* response){
// 	int PID = fork();
// 	if (PID == -1){
// 		close(_fd0);
// 		close(_fd1);
// 		response->autoFillResponse("500 Internal Server Error: fork");
// 		return ;
// 	}
// 	if (PID == 0){ //child (read from _fd0, write to _fd1)
// 		if (request->_method_type == POST)
// 			dup2(_fd0, STDIN_FILENO);
// 		else if (request->_method_type == GET)
// 			dup2(_fd0, STDOUT_FILENO);
// 		dup2(_fd1, STDERR_FILENO);
// 		close(_fd0);
// 		close(_fd1);
// 		response = CGI::executeScript(request, response); // into buffer and then to response or write errors to stderr(file)
// 	}
// 	else{ //parent (read from _fd1, write to _fd0)
// 		if (request->_method_type == POST)
// 			write(_fd0, request->getReqBody().data(), request->getReqBody().size());
// 		else{
// 			char buffer[BUFFER_SIZE];
// 			int bytesRead = 0;
// 			while ((bytesRead = read(_fd0, buffer, BUFFER_SIZE)) > 0)
// 				response->setBody(std::vector<char>(buffer, buffer + bytesRead));
// 		}
// 		std::string errorMessage;
// 		char errorBuffer[BUFFER_SIZE];
// 		int bytesRead = 0;
// 		while ((bytesRead = read(_fd0, errorBuffer, BUFFER_SIZE)) > 0)
// 			errorMessage = std::string(errorBuffer, errorBuffer + bytesRead);
// 		close(_fd1);
// 		// waitpid(PID, NULL, 0); don't wait but timeout?
// 	}
// 	return response;
// }

Response*	CGI::invokeCGI(Request* request, Response* response){
	int PID = fork();
	if (PID == -1){
		close(_fd0);
		close(_fd1);
		response->autoFillResponse("500 Internal Server Error: fork");
		return ;
	}
	if (PID == 0){ //child (read from _fd0, write to _fd1)
		if (request->_method_type == POST)
			dup2(_fd0, STDIN_FILENO);
		dup2(_fd1, STDOUT_FILENO);
		close(_fd0);
		close(_fd1);
		response = CGI::executeScript(request, response); // into buffer and then to response or write errors to stderr(file)
	}
	else{ //parent (read from _fd1, write to _fd0)
		if (request->_method_type == POST)
			write(_fd0, request->getReqBody().data(), request->getReqBody().size());
		close(_fd0);
		char buffer[BUFFER_SIZE];
		int bytesRead = 0;
		while ((bytesRead = read(_fd1, buffer, BUFFER_SIZE)) > 0){
			response->setBody(std::vector<char>(buffer, buffer + bytesRead));

		}
		close(_fd1);
		// waitpid(PID, NULL, 0); don't wait but timeout?
	}
	return response;
}

Response*	CGI::executeScript(Request* request, Response* response){
	char* argv[] = {strdup(request->_filePath.c_str()), NULL};
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
		CGI::addToEnvp("CONTENT_LENGTH", std::to_string(request->_headers["Content-Length"].length()));
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

// int	CGI::getFDIn(void) const{
// 	return _fdIn;
// }

// int	CGI::getFDOut(void) const{
// 	return _fdOut;
// }
