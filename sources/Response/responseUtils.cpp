#include "Response.hpp"
#include "Request.hpp"

bool	fileExists(std::string path){
	std::ifstream file(path);
	if (file.is_open()){
		file.close();
		return true;
	}
	return false;
}
