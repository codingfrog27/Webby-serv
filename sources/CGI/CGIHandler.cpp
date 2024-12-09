#include "CGI.hpp"

//next up: testing
// write script
// put reading and writing in a loop

Response*	CGIHandler(Request* request, Response* response){
	int	fdIn[2];
	int	fdOut[2];
	int fdError[2];

	// should we even invoke CGI?
	if (request->_method_type != GET && request->_method_type != POST){
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
	if (pipe(fdIn) == -1) {
		response->autoFillResponse("500 Internal Server Error: pipe fdIn");
		return response;
	}
	if (pipe(fdOut) == -1) {
		close(fdIn[0]);
		close(fdIn[1]);
		response->autoFillResponse("500 Internal Server Error: pipe fdOut");
		return response;
	}
	if (pipe(fdError) == -1) {
		close(fdIn[0]);
		close(fdIn[1]);
		close(fdOut[0]);
		close(fdOut[1]);
		response->autoFillResponse("500 Internal Server Error: pipe fdError");
		return response;
	}
	CGI* newCGI = new CGI(fdIn, fdOut, fdError);
	newCGI->setupCGIEnvironment(request);
	response = newCGI->invokeCGI(request, response);

	return response;
}
