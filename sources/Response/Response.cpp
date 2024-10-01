#include "Response.hpp"

// do i need to init all values??
Response::Response(HttpRequest* request) : _method_type(request->_method_type), _http_version(request->_http_version){
	_status = "200 OK";
	_headers["Content-Type"] = "text/html";
	_body = "";
}

Response::~Response(){
	delete this;
}

std::string	Response::generateResponse() const{
	std::string response = "HTTP/" + std::to_string(_http_version) + " " + _status + "\r\n";
	for (auto it = _headers.begin(); it != _headers.end(); it++){
		response += it->first + ": " + it->second + "\r\n";
	}
	response += "\r\n";
	response += _body;
	return response;
}

/*	general set content-type function based on extension type	*/
void	setContentType(std::string path, Response* response){
	if (path.find(".html") != std::string::npos || path.find(".htm") != std::string::npos)
		response->setHeaders("Content-Type", "text/html");
	else if (path.find(".css") != std::string::npos)
		response->setHeaders("Content-Type", "text/css");
	else if (path.find(".js") != std::string::npos)
		response->setHeaders("Content-Type", "text/javascript");
	else if (path.find(".jpeg") != std::string::npos || path.find(".jpg") != std::string::npos)
		response->setHeaders("Content-Type", "image/jpeg");
	else if (path.find(".png") != std::string::npos)
		response->setHeaders("Content-Type", "image/png");
	else if (path.find(".gif") != std::string::npos)
		response->setHeaders("Content-Type", "image/gif");
	else if (path.find(".mp3") != std::string::npos)
		response->setHeaders("Content-Type", "audio/mp3");
	else if (path.find(".mp4") != std::string::npos)
		response->setHeaders("Content-Type", "video/mp4");
	else if (path.find(".pdf") != std::string::npos)
		response->setHeaders("Content-Type", "application/pdf");
	else
		response->setHeaders("Content-Type", "text/plain");
}

/*	takes a string as key, finds it in the map and returns the value	*/
std::string	Response::getHeader(std::string key) const{
	auto it = _headers.find(key);
	return it->second;
}
