#pragma once
#include "HttpRequest.hpp"
#include <fstream>

enum readingMode{
	TEXT,
	BINARY
};

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
		const float										_http_version;
		std::string										_status;
		std::unordered_map<std::string, std::string>	_headers;
		std::vector<char>								_body;
};

void		responseHandler(Request* request);
std::string	resolveFilePath(Request* request);
bool		fileExists(std::string path, Response* response);
readingMode	getReadingMode(Response & response);
void		responseHandler(Request* request);