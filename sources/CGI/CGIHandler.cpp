#include "CGI.hpp"

Response*	CGIHandler(Request* request, Response* response){
	int	fd[2];

	// should we even invoke CGI?
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
	CGI* newCGI = new CGI(fd[0], fd[1]);
	newCGI->setupCGIEnvironment(request);
	response = newCGI->invokeCGI(request, response);

	return response;
}

//dup stdout to fd
//raw request data to fd
//dup stdin to fd
//close fd
