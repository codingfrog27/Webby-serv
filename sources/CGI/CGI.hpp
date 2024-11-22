#pragma once
#include "../../include/Response.hpp"

class CGI{
	public:
		CGI() = delete;
		CGI(int fd0, int fd1); // what else do i need to pass?
		CGI(const CGI &obj) = delete;
		CGI& operator=(const CGI& obj) = delete;
		~CGI();

		Response*			invokeCGI(Request* request, Response* response);
		Response*			executeScript(Request* request, Response* response);

		void				setupCGIEnvironment(Request* request);
		void 				addToEnvp(std::string key, std::string value);
		// void				setFDIn(int fd);
		// void				setFDOut(int fd);

		// int					getFDIn(void) const;
		// int					getFDOut(void) const;

	private:
		std::vector<char*>	_envp;
		int					_fd0;
		int					_fd1;

};

Response*	CGIHandler(Request* request, Response* response); 
