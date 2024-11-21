#include "CGI.hpp"

CGI::CGI(int fd, std::unordered_map<std::string, std::string> env) : _fdIn(fd), _env(env){

}

CGI::~CGI(){
}


