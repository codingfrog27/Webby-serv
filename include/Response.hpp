#pragma once
#include <fstream>
#include "Request.hpp"
#include "everything.hpp"

enum readingMode{
	TEXT,
	BINARY
};

// class Request;

class Response{
	public:
		Response() = delete;
		Response(Request* request);
		Response(const Response &obj) = delete;
		Response& operator=(const Response& obj) = delete;
		~Response();

		void											autoFillResponse(std::string status);
		std::string										generateResponse() const;

		void											setStatus(std::string status);
		void											setContentType(std::string path);
		void											setHeaders(std::string key, std::string value);
		void											setBody(std::string body);
		void											setBody(std::vector<char>);

		std::string										getHeader(std::string key) const;

	private:
		const Http_method								_method_type;
		const std::string								_http_version;
		std::string										_status;
		std::unordered_map<std::string, std::string>	_headers;
		std::vector<char>								_body;
};

void		responseHandler(Request* request, Config* config);
// std::string	resolveFilePath(Request* request, Response* response, Config* config);
bool		isCGIrequired(Request* request);
bool		fileExists(std::string path);
readingMode	getReadingMode(Response & response);
