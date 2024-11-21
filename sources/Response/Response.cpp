#include "Response.hpp"

Response::Response(Request* request) : _method_type(request->_method_type), _http_version(request->_http_version){
	return ;
}

Response::~Response(){
	delete this;
}

void	Response::autoFillResponse(std::string status){
	std::string		path = _headers["Root"] + "/error_pages/" + status.substr(0, 3) + ".html";
	size_t			size = 0;
	std::ifstream	file(path);

	Response::setStatus(status);
	if (file.is_open()){
		file.seekg(0, std::ios::end);
		size = file.tellg();
		file.seekg(0, std::ios::beg);
		std::vector<char> buffer(size);
		if (file.read(buffer.data(), size)){
			Response::setContentType(path);
			Response::setHeaders("Content-Length", std::to_string(size));
			Response::setBody(buffer.data());
		}
		else
			Response::autoFillResponse("500 Internal Server Error");
		file.close();
	}
	else{
			Response::setHeaders("Content-Type", "text/plain");
			Response::setHeaders("Content-Length", std::to_string(status.length()));
			Response::setBody(status);
	}
	return ;
}

// std::string tmp_read_file(std::string page)
// {
// 	std::ifstream file(page);
// 	std::stringstream buffer;
// 	if (!file.is_open())
// 	throw std::runtime_error("Could not open file: " + page);	 
// 	buffer << file.rdbuf();
// 	return buffer.str();
// }

std::string	Response::generateResponse() const{
	std::string response = _http_version + " " + _status + "\r\n";
	for (auto it = _headers.begin(); it != _headers.end(); it++){
		response += it->first + ": " + it->second + "\r\n";
	}
	response += "\r\n";
	response += std::string(_body.begin(), _body.end());
	
	return response;
}

/*	Setters	*/

void	Response::setStatus(std::string status){
	_status = status;
	return ;
}
	
void	Response::setContentType(std::string path){
	if (path.find(".html") != std::string::npos || path.find(".htm") != std::string::npos)
		_headers["Content-Type"] = "text/html";
	else if (path.find(".css") != std::string::npos)
		_headers["Content-Type"] = "text/css";
	else if (path.find(".js") != std::string::npos)
		_headers["Content-Type"] = "text/javascript";	
	else if (path.find(".jpeg") != std::string::npos || path.find(".jpg") != std::string::npos)
		_headers["Content-Type"] = "image/jpeg";
	else if (path.find(".png") != std::string::npos)
		_headers["Content-Type"] = "image/png";
	else if (path.find(".gif") != std::string::npos)
		_headers["Content-Type"] = "image/gif";
	else if (path.find(".mp3") != std::string::npos)
		_headers["Content-Type"] = "audio/mp3";
	else if (path.find(".mp4") != std::string::npos)
		_headers["Content-Type"] = "video/mp4";
	else if (path.find(".pdf") != std::string::npos)
		_headers["Content-Type"] = "application/pdf";
	else
		_headers["Content-Type"] = "text/plain";
}

void	Response::setHeaders(std::string key, std::string value){
	_headers[key] = value;
	return ;
}

void	Response::setBody(std::string body){
	_body.assign(body.begin(), body.end());
	return ;
}

void	Response::setBody(std::vector<char> body){
	_body = body;
	return ;
}

/*	getters	*/

/*	takes a string as key, finds it in the map and returns the value	*/
std::string	Response::getHeader(std::string key) const{
	auto it = _headers.find(key);
	return it->second;
}
