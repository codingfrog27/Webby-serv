#include "Response.hpp"
#include "Request.hpp"

// static std::string	checkForRedirection(std::string filePath, Config* config){
// 	if (config->_locNames.contains(filePath)){
// 		return filePath;
// 	}
// }

// here ^^ take into account subdirectories and locations

// std::string	resolveFilePath(Request* request, Response* response, Config* config){
// 	std::string filePath = request->_URI;
// 	std::string temp;

// 	if (filePath.find("?") != std::string::npos)
// 		filePath.erase(filePath.find("?"));
// 	else if (filePath.find("#") != std::string::npos)
// 		filePath.erase(filePath.find("#"));
// 	if (filePath.find("https://") != std::string::npos || filePath.find("http://") != std::string::npos)
// 		filePath.erase(0, filePath.find("//") + 2);
// 	if (filePath.find(config->_host) != std::string::npos)
// 		filePath.erase(0, config->_host.length());
// 	if (filePath.find(config->_serverPort) != std::string::npos)
// 		filePath.erase(0, config->_serverPort.length() + 1);
// 	return config->_rootDir + temp;
// }


bool	isCGIrequired(Request* request){
	if (request == nullptr || request->_filePath == "") {
		std::cout << "AAHHH" << std::endl;
		NicePrint::promptEnter();
		return false;
	}
	if (request->_filePath.rfind(".py") == request->_filePath.length() - 3)
		return true;
	if (request->_headers["Content-Type"].find("multi-part/form-data") != std::string::npos)
		return true;
	if (request->_headers["Content-Type"].find("application/x-www-form-urlencoded") != std::string::npos)
		return true;
	// if (request->_headers["Content-Type"].find("application/json") != std::string::npos)
	// 	return true;
	return false;
}

bool	fileExists(std::string path){
	std::ifstream file(path);
	if (file.is_open()){
		file.close();
		return true;
	}
	return false;
}
