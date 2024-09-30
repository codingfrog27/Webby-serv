#pragma once
#include "../../include/HttpRequest.hpp"

enum readingMode{
	TEXT,
	BINARY
};

class Response{
	public:
		Response() = delete;
		Response(HttpRequest* request);
		Response(const Response &obj) = delete;
		Response& operator=(const Response& obj) = delete;
		~Response();

		std::string										generateResponse() const;

		void											setStatus(std::string status);
		void											setHeaders(std::string header, std::string value);
		void											setBody(std::string body);
		void											setContentType(std::string path, Response* response);

		std::string										getHeader(std::string key) const;

	private:
		const Http_method								_method_type;
		const float										_http_version;
		std::string										_status;
		std::unordered_map<std::string, std::string>	_headers;
		std::string										_body;
};

std::string	resolveFilePath(HttpRequest* request);
bool		fileExists(std::string path, Response* response);
readingMode	getReadingMode(Response & response);