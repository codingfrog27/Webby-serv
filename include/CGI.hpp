#pragma once
#include <vector>
#include <string>
#include <poll.h>
#include <unordered_map>
#include <memory>
#include "timeout.hpp"

class Request;
class Response;
class Connection;

enum class connectStatus;

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
		CGI(const CGI &obj) = delete;
		CGI& operator=(const CGI& obj) = delete;
		CGI(Connection* connection, std::vector<pollfd> &pollFDs);
		~CGI();

		connectStatus		CGIHandler(Connection* connection, std::vector<pollfd> &CGIPollFDs, std::unordered_map<int, std::shared_ptr<CGI>> &CGIMap);
		void				closePipes(void);
		bool				childIsRunning(Response* response);
		bool				CGIisTimedOut(void);
		void				killChild(void);

		void				writeToCGI(Request* request, Response* response);
		void				readFromCGI(Response* response);
		void				readErrorFromCGI(Response* response);

		void				setCGIHandlerStatus(CGIHandlerStatus status);
		CGIHandlerStatus	getCGIHandlerStatus() const;
		bool				getChildIsRunningStatus(void);
		int					getFdIn(void);
		int					getFdOut(void);
		int					getFdError(void);
		int					getClientFD(void);

	private:
		std::vector<char*>	_envp;
		int					_fdIn[2];
		int					_fdOut[2];
		int					_fdError[2];
		int					_clientFD;
		CGIHandlerStatus	_CGIHandlerStatus;
		int					_PID;
		size_t				_bytesWrittenToChild;
		std::string			_scriptError;
		bool				_childIsRunningStatus;
		t_time				_startTime;
		t_secs				_maxDuration;

		void				invokeCGI(Request* request, Response* response);
		void				executeScript(Request* request, Response* response);

		// void				setupPollFds(void);
		void				setupCGIEnvironment(Request* request);
		void 				addToEnvp(std::string key, std::string value);
};

