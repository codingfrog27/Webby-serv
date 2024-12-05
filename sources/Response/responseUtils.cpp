#include "Response.hpp"
#include <fstream>

// static std::string	checkForRedirection(std::string filePath, Config* config){
// 	if (config->_locNames.contains(filePath)){
// 		return filePath;
// 	}
// }

// here ^^ take into account subdirectories and locations



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
