#include "CGI.hpp"

CGI::CGI(int fd) : _fdIn(fd){

}

CGI::~CGI(){
}


void CGI::executeScript(void){
	// dup2(cgi->_fdOut, STDOUT_FILENO);
	// dup2(cgi->_fdIn, STDIN_FILENO);
	// close(cgi->_fdOut);
	// close(cgi->_fdIn);
	// execve(cgi->_env["SCRIPT_FILENAME"].c_str(), NULL, NULL);
}

void CGI::setupCGIEnvironment(Request* request) {
	if (request->_method_type == GET) {
		_env["REQUEST_METHOD"] = "GET";
		if (request->_URI.find("?") != std::string::npos) {
			_env["QUERY_STRING"] = request->_URI.substr(request->_URI.find("?") + 1);
		}
	} 
	else if (request->_method_type == POST) {
		_env["REQUEST_METHOD"] = "POST";
		_env["CONTENT_TYPE"] = request->_headers["Content-Type"];
		_env["CONTENT_LENGTH"] = std::to_string(request->_headers["Content-Length"].length());
	}
	_env["SCRIPT_FILENAME"] = request->_filePath;
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_NAME"] = request->_headers["Host"];
	return ;
}
