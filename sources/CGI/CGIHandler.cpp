#include "CGI.hpp"

static void executeScript(CGI* cgi){
	// dup2(cgi->_fdOut, STDOUT_FILENO);
	// dup2(cgi->_fdIn, STDIN_FILENO);
	// close(cgi->_fdOut);
	// close(cgi->_fdIn);
	// execve(cgi->_env["SCRIPT_FILENAME"].c_str(), NULL, NULL);
}

static std::unordered_map<std::string, std::string> setupCGIEnvironment(Request* request) {
	std::unordered_map<std::string, std::string> env;

	if (request->_method_type == GET) {
		env["REQUEST_METHOD"] = "GET";
		if (request->_URI.find("?") != std::string::npos) {
			env["QUERY_STRING"] = request->_URI.substr(request->_URI.find("?") + 1);
		}
	} 
	else if (request->_method_type == POST) {
		env["REQUEST_METHOD"] = "POST";
		env["CONTENT_TYPE"] = request->_headers["Content-Type"];
		env["CONTENT_LENGTH"] = std::to_string(request->_headers["Content-Length"].length());
	}
	env["SCRIPT_FILENAME"] = request->_filePath;
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_NAME"] = request->_headers["Host"];
	return env;
}

Response*	CGIHandler(Request* request, Response* response, int fd){
	int												fd[2];
	std::unordered_map<std::string, std::string>	env;

	if (request->_method_type != GET || request->_method_type != POST){
		response->autoFillResponse("405 Method Not Allowed");
		response->setHeaders("Allow", "GET, POST");
		return ;
	}
	if (request->_filePath.find("/cgi-bin/", 0) == std::string::npos){
		response->autoFillResponse("403 Forbidden");
		return ;
	}
	if (!fileExists(request->_filePath)){
		response->autoFillResponse("404 Not Found");
		return ;
	}
	if (pipe(fd) == -1 || ){
		response->autoFillResponse("500 Internal Server Error: pipe");
		return ;
	}
	int PID = fork();
	if (PID == -1){
		response->autoFillResponse("500 Internal Server Error: fork");
		return ;
	}
	env = setupCGIEnvironment(request);
	CGI* newCGI = new CGI(fd, env);
	return response;
}

//dup stdout to fd
//raw request data to fd
//dup stdin to fd
//close fd