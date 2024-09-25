#pragma once
#include "../../include/HttpRequest.hpp"

class CGI{
	public:
		CGI() = delete;
		CGI(int fd); // what else do i need to pass?
		CGI(const CGI &obj) = delete;
		CGI& operator=(const CGI& obj) = delete;
		~CGI();

	private:
		int	_fdIn;
		int	_fdOut;
};
