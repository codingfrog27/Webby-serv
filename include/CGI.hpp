#pragma once
#include <vector>

class Request;
class Response;

enum class CGIHandlerStatus{
	NOT_STARTED,
	IN_PROGRESS,
	WRITING_TO_CHILD,
	WAITING_FOR_CHILD,
	READING_FDOUT,
	READING_FDERROR,
	FINISHED
};

class CGI{
	public:
		CGI() = delete;
		CGI(int *fdIn, int *fdOut, int *fdError); // what else do i need to pass?
		CGI(const CGI &obj) = delete;
		CGI& operator=(const CGI& obj) = delete;
		~CGI();

		void				invokeCGI(Request* request, Response* response);
		void				executeScript(Request* request, Response* response);
		void				closePipes();

		void				setupCGIEnvironment(Request* request);
		void 				addToEnvp(std::string key, std::string value);

		void				setCGIHandlerStatus(CGIHandlerStatus status);
		CGIHandlerStatus	getCGIHandlerStatus() const;

	private:
		std::vector<char*>	_envp;
		int					*_fdIn;
		int					*_fdOut;
		int					*_fdError;
		CGIHandlerStatus	_CGIHandlerStatus;
		int					_PID;
		size_t				_bytesWrittenToChild;
		std::string			_scriptError;
};

void	CGIHandler(Request* request, Response* response);
