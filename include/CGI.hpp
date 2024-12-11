#pragma once
#include "Response.hpp"

class CGI{
	public:
		CGI() = delete;
		CGI(int *fdIn, int *fdOut, int *fdError); // what else do i need to pass?
		CGI(const CGI &obj) = delete;
		CGI& operator=(const CGI& obj) = delete;
		~CGI();

		std::string			invokeCGI(Request* request, Response* response);
		Response*			executeScript(Request* request, Response* response);
		void				closePipes();

		void				setupCGIEnvironment(Request* request);
		void 				addToEnvp(std::string key, std::string value);

	private:
		std::vector<char*>	_envp;
		int					*_fdIn;
		int					*_fdOut;
		int					*_fdError;

};

std::string	CGIHandler(Request* request, Response* response);
