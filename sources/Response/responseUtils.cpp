#include "Response.hpp"
#include <fstream>

std::string	resolveFilePath(Request* request){
	std::string uri = request->_URI;
	// std::string host = request->_headers["Host"]; //
	// std::string file = uri.erase(0, host.length()); ///host length + 8 if http:// or 9 if https:// with request target in absolute form
	// std::string path = request->_headers["Root"] + file; 
	std::string path = "website" + uri; //needs to be grabbed from config, not request
	return path;
}

bool	fileExists(std::string path, Response* response){
	std::ifstream file(path);
	if (file.is_open()){
		file.close();
		return true;
	}
	return false;
	(void)response; //fix later
}

/* anything that's not text has to be handled as raw bits*/
readingMode	getReadingMode(Response & response){
	if (response.getHeader("Content-type").find("text") != std::string::npos)
		return TEXT;
	else
		return BINARY;
}
