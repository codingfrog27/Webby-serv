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
			file.seekg(0, std::ios::end);
			size = file.tellg();
			file.seekg(0, std::ios::beg);
			std::vector<char> buffer(size);
			if (file.read(buffer.data(), size)){
				response->setBody(buffer.data());
				response->setHeaders("Content-Length", std::to_string(size));
			}
			// else throw 500;
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
	std::string path = resolveFilePath(request);
	std::ofstream file;

	// check for CGI??
	if(getReadingMode(*response) == TEXT)
		file.open(path, std::ios::app);
	else
		file.open(path);
	if (file.is_open()){
		file << request->getRawRequestData; //get body
		file.close();
		response->setStatus("201 Created");
		response->setHeaders("Content-Type", "text/html");
		response->setHeaders("Content-Length", "13");
		response->setBody("201 Created");
	}
	else{
		response->setStatus("500 Internal Server Error");
		response->setHeaders("Content-Type", "text/html");
		response->setHeaders("Content-Length", "24");
		response->setBody("Internal Server Error");
	}
}

static Response*	deleteMethod(HttpRequest* request, Response* response){
	std::string path = resolveFilePath(request);

	if (fileExists(path, response)){
		if (std::remove(path.c_str()) == 0){
			response->setStatus("200 OK");
			response->setHeaders("Content-Type", "text/html");
			response->setHeaders("Content-Length", "9");
			response->setBody("200 OK");
		}
		else{
			response->setStatus("500 Internal Server Error");
			response->setHeaders("Content-Type", "text/html");
			response->setHeaders("Content-Length", "24");
			response->setBody("Internal Server Error");
		}
	}
	else{
		response->setStatus("404 Not Found");
		response->setHeaders("Content-Type", "text/html");
		response->setHeaders("Content-Length", "14");
		response->setBody("404 Not Found");
	}
}

void	responseHandler(HttpRequest* request)
{
	Response *response = new Response(request);
	//cgi handler
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
