#include "Response.hpp"

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
				response->setStatus("200 OK");
				response->setContentType(path);
				response->setHeaders("Content-Length", std::to_string(size));
				response->setBody(buffer);
			}
			file.close();
		}
		else
			response->autoFillResponse("500 Internal Server Error");
	}
	else
		response->autoFillResponse("404 Not Found");
	return response;
}

static Response*	postMethod(HttpRequest* request, Response* response){
	std::string path = resolveFilePath(request);
	std::ofstream file;

	// check for CGI??
	if(getReadingMode(*response) == BINARY)
		file.open(path, std::ios::app);
	else
		file.open(path);
	if (file.is_open()){
		// file << request->getRawRequestData; //get body
		file.close();
		response->autoFillResponse("201 Created");
	}
	else
		response->autoFillResponse("500 Internal Server Error");
	return response;
}

static Response*	deleteMethod(HttpRequest* request, Response* response){
	std::string path = resolveFilePath(request);

	if (fileExists(path, response)){
		if (std::remove(path.c_str()) == 0)
			response->autoFillResponse("200 OK");
		else
			response->autoFillResponse("500 Internal Server Error");
	}
	else
		response->autoFillResponse("404 Not Found");
	return response;
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
		response->autoFillResponse("405 Method Not Allowed");
		response->setHeaders("Allow", "GET, POST, DELETE");
	}
	std::cout << response->generateResponse() << std::endl;
	return;
}
