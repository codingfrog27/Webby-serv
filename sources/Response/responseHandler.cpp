#include "Response.hpp"

// decide repsonse based on request

std::string resolveFilePath(HttpRequest* request)
{
	std::string url = request->_URI;
	std::string host = request->_headers["Host"];
	url.erase(0, host.length()+8);
	return url;
}

https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c

//check file exists

// Response::generateResponse(httpRequest);