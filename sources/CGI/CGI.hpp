#pragma once
#include "../../include/Response.hpp"

class CGI{
	public:
		CGI() = delete;
		CGI(int fd); // what else do i need to pass?
		CGI(const CGI &obj) = delete;
		CGI& operator=(const CGI& obj) = delete;
		~CGI();

		void											executeScript();

		void											setupCGIEnvironment(Request* request);
		void											setFDIn(int fd);
		void											setFDOut(int fd);

		int												getFDIn(void) const;
		int												getFDOut(void) const;

	private:
		std::unordered_map<std::string, std::string>	_env;
		int												_fdIn;
		int												_fdOut;

};

Response*	CGIHandler(Request* request, Response* response, int fd); 
