#include "CGI.hpp"
#include "Response.hpp"
#include "Request.hpp"

//next up: testing
// write script
// put reading and writing in a loop

void	CGIHandler(Request* request, Response* response){
	// std::cout << "in CGI???" << std::endl;
	// NicePrint::promptEnter();
	// if (response->getResponseHandlerStatus() == responseHandlerStatus::IN_PROGRESS){
		// should we even invoke CGI?
		if (request->_method_type != GET && request->_method_type != POST){
			response->autoFillResponse("405 Method Not Allowed");
			response->setHeaders("Allow", "GET, POST");
			return ;
		}
		if (request->_filePath.find("cgi-bin/", 0) == std::string::npos){
			response->autoFillResponse("403 Forbidden");
			return ;
		}
		if (!fileExists(request->_filePath)){
			response->autoFillResponse("404 Not Found: CGI");
			return ;
		}
		// if yes
		CGI* newCGI = new CGI(fdIn, fdOut, fdError);
		// newCGI->setupCGIEnvironment(request);
		// response->setCGI(newCGI);
		//how do i pushback the cgiPollFD's to the _pollFDs vector?
	// }
	CGI* CGI = response->getCGI();
	CGI->invokeCGI(request, response);
	//set cgi in map
	if (CGI->getCGIHandlerStatus() == CGIHandlerStatus::FINISHED && response->getResponseHandlerStatus() == responseHandlerStatus::READY_TO_WRITE)
		delete CGI;
	return ;
}

//move piping to if statement so it's only run once
//in invokeCGI check if child is running
// if no, fork
//if yes, check if child is done
// if child is done read & write from pipe
