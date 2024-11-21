#include "Response.hpp"

static Response*	getMethod(Request* request, Response* response, std::string filePath){
	std::ifstream file;
	size_t size = 0;

	if (fileExists(filePath)){
		response->setContentType(filePath);
		if (getReadingMode(*response) == BINARY)
			file.open(filePath, std::ios::binary); 
		else
			file.open(filePath);
		if (file.is_open()){
			file.seekg(0, std::ios::end);
			size = file.tellg();
			if (size == 0){
				file.close();
				response->autoFillResponse("204 No Content");
				return response;
			}
			file.seekg(0, std::ios::beg);
			std::vector<char> buffer(size);
			if (file.read(buffer.data(), size)){
				response->setStatus("200 OK");
				response->setHeaders("Content-Length", std::to_string(size));
				response->setBody(buffer);
			}
			file.close();
		}
		else
			response->autoFillResponse("500 Internal Server Error");
	}
	else
		response->autoFillResponse("404 Not Found, AUTO INDEX SOON"); //ye
	return (response);
}

static Response*	postMethod(Request* request, Response* response, std::string filePath){
	std::ofstream file;

	// check for CGI??
	if(getReadingMode(*response) == BINARY)
		file.open(filePath, std::ios::app);
	else
		file.open(filePath);
	if (file.is_open()){
		// file << request->getRawRequestData; //get body
		file.close();
		response->autoFillResponse("201 Created");
	}
	else
		response->autoFillResponse("500 Internal Server Error");
	return response;
}

static Response*	deleteMethod(Request* request, Response* response, std::string filePath){

	if (fileExists(filePath)){
		if (std::remove(filePath.c_str()) == 0)
			response->autoFillResponse("200 OK");
		else
			response->autoFillResponse("500 Internal Server Error");
	}
	else
		response->autoFillResponse("404 Not Found");
	return response;
}

void	responseHandler(Request* request, Config* config)
{
	Response *response = new Response(request);
	std::string filePath = resolveFilePath(request, response, config);
	std::string responseText;

	if (!request->getStatusCode().empty()) //if there was an error in (parsing) the request
		response->autoFillResponse(request->getStatusCode());
	else if (request->_method_type == GET)
		response = getMethod(request, response, filePath);
	else if (request->_method_type == POST)
		response = postMethod(request, response, filePath);
	else if (request->_method_type == DELETE)
		response = deleteMethod(request, response, filePath);
	else{
		response->autoFillResponse("405 Method Not Allowed");
		response->setHeaders("Allow", "GET, POST, DELETE");
	}
	responseText = response->generateResponse();
	std::cout << responseText << std::endl;
	write(request->_clientFD, responseText.c_str(), responseText.size()); //needs to be send back in a loop (see requestHandler)
	return;
}
