#include "CGI.hpp"

static int	executeScript(std::unordered_map<std::string, std::string> env){
	//dup stdout to fd
	//raw request data to fd
	//dup stdin to fd
	//close fd
}

CGI::CGI(int fd, std::unordered_map<std::string, std::string> env) : _fdIn(fd), _env(env){

}

CGI::~CGI(){
}


