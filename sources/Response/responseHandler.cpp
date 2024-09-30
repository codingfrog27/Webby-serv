#include "Response.hpp"
#include <fstream>

static Response*	getMethod(HttpRequest* request, Response* response){
	std::string path = resolveFilePath(request);
	std::ifstream file;
	size_t size = 0;

	if (fileExists(path, response)){
		if (getReadingMode(*response) == BINARY)
			file.open(path, std::ios::binary);
		else
			file.open(path);
		if (file.is_open()){
			size = file.tellg();
			file.seekg(0, std::ios::beg);
			char buffer[size];
			if (file.read(buffer, size)){
				response->setBody(buffer);
				response->setHeaders("Content-Length", std::to_string(size));
			}
			file.close();
		}
	}
	else{
		response->setStatus("404 Not Found");
		response->setHeaders("Content-Type", "text/html");
		response->setHeaders("Content-Length", "14");
		response->setBody("404 Not Found");
	}
}

static Response*	postMethod(HttpRequest* request, Response* response){
}

static Response*	deleteMethod(HttpRequest* request, Response* response){
}

void	responseHandler(HttpRequest* request)
{
	Response *response = new Response(request);
	if (request->_method_type == GET)
		response = getMethod(request, response);
	else if (request->_method_type == POST)
		response = postMethod(request, response);
	else if (request->_method_type == DELETE)
		response = deleteMethod(request, response);
	else{
		response->setStatus("405 Method Not Allowed");
		response->setHeaders("Allow", "GET, POST, DELETE");
		response->setHeaders("Content-Type", "text/html");
		response->setHeaders("Content-Length", "19");
		response->setBody("Method Not Allowed");
	}
	response->generateResponse();
	return;
}
