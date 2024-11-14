#include "Response.hpp"
#include <fstream>

static std::string	checkForRedirection(std::string filePath, Config* config){
	if (config->_locNames.contains(filePath)){
		return filePath;
	}
}

// here ^^ take into account subdirectories and locations

std::string	resolveFilePath(Request* request, Response* response, Config* config){
	std::string filePath = request->_URI;
	std::string temp;

	if (filePath.find("?") != std::string::npos)
		filePath.erase(filePath.find("?"));
	else if (filePath.find("#") != std::string::npos)
		filePath.erase(filePath.find("#"));
	if (filePath.find("https://") != std::string::npos || filePath.find("http://") != std::string::npos)
		filePath.erase(0, filePath.find("//") + 2);
	if (filePath.find(config->_host) != std::string::npos)
		filePath.erase(0, config->_host.length());
	if (filePath.find(config->_serverPort) != std::string::npos)
		filePath.erase(0, config->_serverPort.length() + 1);
	temp = checkForRedirection(filePath, config);
	if (temp != filePath){
		response->setStatus("301 Moved Permanently");
	}
		return temp;
	return config->_rootDir + temp;
}

bool	fileExists(std::string path){
	std::ifstream file(path);
	if (file.is_open()){
		file.close();
		return true;
	}
	return false;
}

/* anything that's not text has to be handled as raw bits*/
readingMode	getReadingMode(Response & response){
	if (response.getHeader("Content-type").find("text") != std::string::npos)
		return TEXT;
	else
		return BINARY;
}
