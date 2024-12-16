#pragma once
#include <fstream>
#include <string.h>
#include "Request.hpp"

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
		void											setStatus(std::string status);
		void											setContentType(std::string path);
		void											setHeaders(std::string key, std::string value);
		void											setBody(std::string body);
		void											setBody(std::vector<char>);
		void											setResponseBuffer(std::string buffer);

		responseHandlerStatus							getResponseHandlerStatus() const;
		std::string										getHeader(std::string key) const;
		readingMode										getReadingModeFromRequest(Request & request) const;
		readingMode										getReadingModeFromResponse() const;
		std::string										getResponseBuffer() const;

	private:
		responseHandlerStatus							_responseHandlerStatus;
		std::string										_httpVersion;
		std::string										_status;
		std::unordered_map<std::string, std::string>	_headers;
		std::string										_body;
		std::string										_responseBuffer;
};

void		responseHandler(Request* request, Response* response, Config* config);
bool		isCGIrequired(Request* request);
bool		fileExists(std::string path);
