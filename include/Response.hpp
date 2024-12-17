#pragma once
#include <fstream>
#include <string.h>
#include "Request.hpp"
#include "CGI.hpp"

enum readingMode{
	TEXT,
	BINARY
};

enum class responseHandlerStatus{
	NOT_STARTED,
	IN_PROGRESS,
	IN_CGI,
	IN_GET,
	IN_POST,
	IN_DELETE,
	READY_TO_WRITE,
	WRITING,
	FINISHED
};

class Response{
	public:
		Response();
		Response(const Response &obj) = delete;
		Response& operator=(const Response& obj) = delete;
		~Response();

		void											autoFillResponse(std::string status);
		std::string										generateResponse() const;

		void											setHTTPVersion(std::string HTTPversion);
		void											setResponseHandlerStatus(responseHandlerStatus status);
		void											setCGI(CGI* cgi);
		void											setStatus(std::string status);
		void											setContentType(std::string path);
		void											setHeaders(std::string key, std::string value);
		void											setBody(std::string body);
		void											setBody(std::vector<char>);
		void											setResponseBuffer(std::string buffer);
		void											setBytesWritten(size_t bytesWritten);

		responseHandlerStatus							getResponseHandlerStatus() const;
		CGI*											getCGI() const;
		std::string										getHeader(std::string key) const;
		readingMode										getReadingModeFromRequest(Request & request) const;
		readingMode										getReadingModeFromResponse() const;
		std::string										getResponseBuffer() const;
		size_t											getBytesWritten() const;

	private:
		responseHandlerStatus							_responseHandlerStatus;
		std::string										_httpVersion;
		CGI*											_cgi;
		std::string										_status;
		std::unordered_map<std::string, std::string>	_headers;
		std::string										_body;
		std::string										_responseBuffer;
		size_t											_bytesWritten;
};

void		responseHandler(Request* request, Response* response, Config* config);
bool		isCGIrequired(Request* request);
bool		fileExists(std::string path);
