#pragma once
#include "../../include/HttpRequest.hpp"

class Response{
	public:
		Response() = delete;
		Response(HttpRequest* request);
		Response(const Response &obj) = delete;
		Response& operator=(const Response& obj) = delete;
		~Response();

		std::string		generateResponse();

		void			setStatus(std::string status);
		void			setHeaders(std::string headers);
		void			setBody(std::string body);

	private:
		std::string		_status;
		std::string		_headers;
		std::string		_body;
};
