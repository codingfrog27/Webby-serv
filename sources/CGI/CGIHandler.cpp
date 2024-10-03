#include "CGI.hpp"

static bool	isCGIrequiredPOST(HttpRequest* request){
	// possibly multipart/form-data as well or application/json
	if (request->_headers["Content-Type"].find("application/x-www-form-urlencoded") != std::string::npos)
		return true;
	return false;
}

static bool	isCGIrequiredGET(HttpRequest* request){
	if (request->_URI.find(".py") != std::string::npos)
		return true;
	return false;
}

void	CGIHandler(HttpRequest* request, Response* response, int fd){
	bool CGIrequired = false;
	std::unordered_map<std::string, std::string>	env;

	if (request->_method_type == GET){
		CGIrequired = isCGIrequiredGET(request);
		if (CGIrequired){
			env["REQUEST_METHOD"] = "GET";
			env["QUERY_STRING"] = request->_URI.substr(request->_URI.find("?") + 1);
			env["SCRIPT_NAME"] = request->_URI.substr(0, request->_URI.find("?"));
		}
	}
	else if (request->_method_type == POST){
		CGIrequired = isCGIrequiredPOST(request);
		if (CGIrequired){
			env["REQUEST_METHOD"] = "POST";
			env["CONTENT_TYPE"] = request->_headers["Content-Type"];
			env["CONTENT_LENGTH"] = std::to_string(request->_headers["Content-Length"].length());
			env["SCRIPT_NAME"] = request->_URI;
		}
	}
	if (CGIrequired){
		env["SERVER_PROTOCOL"] = "HTTP/1.1";
		env["SERVER_NAME"] = request->_headers["Host"];
		CGI* newCGI = new CGI(fd, env);
	}
}
