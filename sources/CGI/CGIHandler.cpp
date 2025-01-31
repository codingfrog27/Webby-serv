#include "CGI.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Connection.hpp"

//next up: testing
// write script
// put reading and writing in a loop

connectStatus	CGI::CGIHandler(Connection* connection, std::vector<pollfd>* CGIPollFDs, std::unordered_map<int, std::shared_ptr<CGI>> CGIMap){
	if (connection == nullptr || CGIPollFDs == nullptr) {
		std::cerr << "Null pointer detected in CGIHandler" << std::endl;
		return connectStatus::SERVER_ERR;
	}

	Response* response = &connection->_response;
	Request* request = &connection->_request;

	if (_CGIHandlerStatus == CGIHandlerStatus::NOT_STARTED){
		_CGIHandlerStatus = CGIHandlerStatus::IN_PROGRESS;
		if (request->_method_type != GET && request->_method_type != POST){
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
	}
	return connectStatus::CGI;
}
