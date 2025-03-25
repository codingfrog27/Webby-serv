#include "CGI.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Connection.hpp"

//next up: testing
// write script
// put reading and writing in a loop

connectStatus	CGI::CGIHandler(Connection* connection, std::vector<pollfd> &CGIPollFDs, std::unordered_map<int, std::shared_ptr<CGI>> &CGIMap){
	Response* response = &connection->_response;
	Request* request = &connection->_request;

		response->setHeaders("Root", request->getRoot());
	if (connection->_cgi == 0 && connection->_CStatus == connectStatus::CGI_REQUIRED){
		if (request->_method_type != Http_method::GET && request->_method_type != Http_method::POST){
			response->autoFillResponse("405 Method Not Allowed");
			response->setHeaders("Allow", "GET, POST");
			return connectStatus::RESPONDING;
		}
		if (request->_filePath.find("cgi-bin/", 0) == std::string::npos){
			response->autoFillResponse("403 Forbidden");
			return connectStatus::RESPONDING;
		}
		if (!fileExists(request->_filePath)){
			response->autoFillResponse("404 Not Found: CGI");
			return connectStatus::RESPONDING;
		}
		// if yes
		std::shared_ptr<CGI> newCGI = std::make_shared<CGI>(connection, CGIPollFDs);
		connection->_cgi = newCGI;
		CGIMap[newCGI->getFdIn()] = newCGI;
		CGIMap[newCGI->getFdOut()] = newCGI;
		CGIMap[newCGI->getFdError()] = newCGI;
		newCGI->invokeCGI(request, response);

		std::cout << MAGENTA "CGI PollFD vector size in CGIHandler: " << CGIPollFDs.size() << RESET << std::endl;
	}
	return connectStatus::CGI;
}
