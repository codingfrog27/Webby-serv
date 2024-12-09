#include "Response.hpp"
#include "CGI.hpp"

static Response*	getMethod(Request* request, Response* response){
	std::ifstream file;
	size_t size = 0;

	if (fileExists(request->_filePath)){
		response->setContentType(request->_filePath);
		if (response->getReadingMode() == BINARY)
			file.open(request->_filePath, std::ios::binary); 
		else
			file.open(request->_filePath);
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

static Response*	postMethod(Request* request, Response* response){
	std::ofstream file;

	// check for CGI??
	if(response->getReadingMode(*request) == BINARY)
		file.open(request->_filePath, std::ios::binary);
	else
		file.open(request->_filePath);
	if (file.is_open()){
		file << request->getBody();
		file.close();
		response->autoFillResponse("201 Created");
	}
	else
		response->autoFillResponse("500 Internal Server Error");
	return response;
}

static Response*	deleteMethod(Request* request, Response* response){

	if (fileExists(request->_filePath)){
		if (std::remove(request->_filePath.c_str()) == 0)
			response->autoFillResponse("200 OK");
		else
			response->autoFillResponse("500 Internal Server Error");
	}
	else
		response->autoFillResponse("404 Not Found");
	return response;
}

//config for timeout & max body size
void	responseHandler(Request* request, Config* config)
{
	Response *response = new Response(request);
	std::string responseBuffer;

	if (!request->getStatusCode().empty()) //if there was an error in (parsing) the request
		response->autoFillResponse(request->getStatusCode());
	else if (isCGIrequired(request))
		response = CGIHandler(request, response);
	else if (request->_method_type == GET)
		response = getMethod(request, response);
	else if (request->_method_type == POST)
		response = postMethod(request, response);
	else if (request->_method_type == DELETE)
		response = deleteMethod(request, response);
	// else{
	// 	response->autoFillResponse("405 Method Not Allowed");
	// 	response->setHeaders("Allow", "GET, POST, DELETE");
	// }
	// to be handled in requestHandler
	responseBuffer = response->generateResponse();
	std::cout << responseBuffer << std::endl;
	write(request->_clientFD, responseBuffer.c_str(), responseBuffer.size()); //needs to be send back in a loop (see requestHandler)
	return;
	(void)config;
}



