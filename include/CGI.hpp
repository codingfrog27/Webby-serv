#pragma once
#include <vector>
#include <string>
#include <poll.h>

class Request;
class Response;
class Connection;

enum class CGIHandlerStatus{
	NOT_STARTED,
	IN_PROGRESS,
	WRITING_TO_CHILD,
	WAITING_FOR_CHILD,
	READING_FDOUT,
	READING_FDERROR,
	CHILD_IS_FINISHED,
	FINISHED
};

class CGI{
	public:
		CGI() = delete;
		CGI(const CGI &obj) = delete;
		CGI& operator=(const CGI& obj) = delete;
		// CGI(int *fdIn, int *fdOut, int *fdError); // what else do i need to pass?
		CGI(Connection* connection, std::vector<pollfd>* pollFDs);
		~CGI();

		void				invokeCGI(Request* request, Response* response);
		void				executeScript(Request* request, Response* response);
		bool				childIsRunning(Response* response);
		void				closePipes(void);

		// void				setupPollFds(void);
		void				setupCGIEnvironment(Request* request);
		void 				addToEnvp(std::string key, std::string value);

		void				writeToCGI(Request* request, Response* response);
		void				readFromCGI(Response* response);
		void				readErrorFromCGI(Response* response);

		void				setCGIHandlerStatus(CGIHandlerStatus status);
		CGIHandlerStatus	getCGIHandlerStatus() const;
		int					getFdIn(void);
		int					getFdOut(void);
		int					getFdError(void);

	private:
		std::vector<char*>	_envp;
		int					*_fdIn;
		int					*_fdOut;
		int					*_fdError;
		// pollfd				_pollFdIn;
		// pollfd				_pollFdOut;
		// pollfd				_pollFdError;
		CGIHandlerStatus	_CGIHandlerStatus;
		int					_PID;
		size_t				_bytesWrittenToChild;
		std::string			_scriptError;
};

void	CGIHandler(Request* request, Response* response);
