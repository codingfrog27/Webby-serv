#include "CGI.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Connection.hpp"

connectStatus	CGI::CGIHandler(Connection* connection, std::vector<pollfd> &CGIPollFDs, std::unordered_map<int, std::shared_ptr<CGI>> &CGIMap){
	Response* response = &connection->_response;
	Request* request = &connection->_request;

	if (connection->_cgi == 0 && connection->_CStatus == connectStatus::CGI_REQUIRED){
		if (request->_method_type != Http_method::GET && request->_method_type !=  Http_method::POST){
			request->_statusCode = 405;
			request->_statusStr = "405 Method Not Allowed";
			request->_filePath = response->getRoot() + "cgi-bin/error.js";
			request->_method_type = Http_method::GET;
			return connectStatus::CGI_REQUIRED;
		}
		if (request->_filePath.find("cgi-bin/", 0) == std::string::npos){
			request->_statusCode = 403;
			request->_statusStr = "403 Forbidden";
			request->_filePath = response->getRoot() + "cgi-bin/error.js";
			request->_method_type = Http_method::GET;
			return connectStatus::CGI_REQUIRED;
		}
		if (!fileExists(request->_filePath)){
			request->_statusCode = 404;
			request->_statusStr = "404 Not Found";
			request->_filePath = response->getRoot() + "cgi-bin/error.js";
			request->_method_type = Http_method::GET;
			return connectStatus::CGI_REQUIRED;
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
