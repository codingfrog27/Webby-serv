#pragma once
#include <fstream>
#include <string.h>
#include "Request.hpp"

enum readingMode{
	TEXT,
	BINARY
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
		void											setStatus(std::string status);
		void											setContentType(std::string path);
		void											setHeaders(std::string key, std::string value);
		void											setBody(std::string body);
		void											setBody(std::vector<char>);

		std::string										getHeader(std::string key) const;
		readingMode										getReadingModeFromRequest(Request & request) const;
		readingMode										getReadingModeFromResponse() const;

	private:
		std::string										_httpVersion;
		std::string										_status;
		std::unordered_map<std::string, std::string>	_headers;
		std::string										_body;
};

void		responseHandler(Request* request, Response* response, Config* config);
bool		isCGIrequired(Request* request);
bool		fileExists(std::string path);
