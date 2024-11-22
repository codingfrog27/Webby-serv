#include "CGI.hpp"

Response*	CGIHandler(Request* request, Response* response, int fd){
	int	fd[2];

	// should we even envoke CGI?
	if (request->_method_type != GET || request->_method_type != POST){
		response->autoFillResponse("405 Method Not Allowed");
		response->setHeaders("Allow", "GET, POST");
		return ;
	}
	if (request->_filePath.find("/cgi-bin/", 0) == std::string::npos){
		response->autoFillResponse("403 Forbidden");
		return ;
	}
	if (!fileExists(request->_filePath)){
		response->autoFillResponse("404 Not Found");
		return ;
	}
	// if yes
	if (pipe(fd) == -1){
		response->autoFillResponse("500 Internal Server Error: pipe");
		return ;
	}
	int PID = fork();
	if (PID == -1){
		response->autoFillResponse("500 Internal Server Error: fork");
		return ;
	}
	CGI* newCGI = new CGI(fd);
	newCGI->setupCGIEnvironment(request);
	return response;
}

//dup stdout to fd
//raw request data to fd
//dup stdin to fd
//close fd