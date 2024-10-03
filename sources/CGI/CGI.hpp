#pragma once
#include "../../include/Response.hpp"

class CGI{
	public:
		CGI() = delete;
		CGI(int fd, std::unordered_map<std::string, std::string> env); // what else do i need to pass?
		CGI(const CGI &obj) = delete;
		CGI& operator=(const CGI& obj) = delete;
		~CGI();

	private:
		int												_fdIn;
		int												_fdOut;
		std::unordered_map<std::string, std::string>	_env;

};

void	CGIHandler(HttpRequest* request, Response* response, Http_method method); 
