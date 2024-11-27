#pragma once
#include "Response.hpp"

class CGI{
	public:
		CGI() = delete;
		CGI(int *fdIn, int *fdOut, int *fdError); // what else do i need to pass?
		CGI(const CGI &obj) = delete;
		CGI& operator=(const CGI& obj) = delete;
		~CGI();

		Response*			invokeCGI(Request* request, Response* response);
		Response*			executeScript(Request* request, Response* response);
		void				closePipes();

		void				setupCGIEnvironment(Request* request);
		void 				addToEnvp(std::string key, std::string value);

	private:
		std::vector<char*>	_envp;
		int					_fdIn[2];
		int					_fdOut[2];
		int					_fdError[2];

};

Response*	CGIHandler(Request* request, Response* response);
