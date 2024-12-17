#include "Response.hpp"
#include "CGI.hpp"

static Response*	getMethod(Request* request, Response* response){
	std::ifstream file;
	size_t size = 0;

	if (fileExists(request->_filePath)){
		response->setContentType(request->_filePath);
		if (response->getReadingModeFromResponse() == BINARY)
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
			std::unique_ptr<std::vector<char>> buffer = std::make_unique<std::vector<char>>(size);
			if (file.read(buffer->data(), size)){
				response->setStatus("200 OK");
				response->setHeaders("Content-Length", std::to_string(size));
				response->setBody(*buffer);
			}
			file.close();
		}
		else
			response->autoFillResponse("500 Internal Server Error: GET");
	}
	else
		response->autoFillResponse("404 Not Found, AUTO INDEX SOON"); //ye
	return (response);
}

static Response*	postMethod(Request* request, Response* response){
	std::ofstream file;

	// check for CGI??
	if(response->getReadingModeFromRequest(*request) == BINARY)
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
void	responseHandler(Request* request, Response* response, Config* config)
{
	if (response->getResponseHandlerStatus() == responseHandlerStatus::NOT_STARTED){
		response->setResponseHandlerStatus(responseHandlerStatus::IN_PROGRESS);
		response->setHTTPVersion(request->_http_version);
	}
	(void)config;
	if (response->getResponseHandlerStatus() == responseHandlerStatus::IN_PROGRESS && !request->getStatusCode().empty()) //if there was an error in (parsing) the request{}
		response->autoFillResponse(request->getStatusCode());
	// std::cout << MAGENTA "Method		: " << request->_method_type << " (0 = GET, 1 = POST, 2 = DELETE)" RESET << std::endl;
	// std::cout << MAGENTA "Content-type	: " << request->getHeaderValue("Content-Type") << RESET << std::endl;
	// std::cout << MAGENTA "filepath	: " << request->_filePath << RESET << std::endl;
	if (response->getResponseHandlerStatus() == responseHandlerStatus::IN_CGI || isCGIrequired(request))
		CGIHandler(request, response); //FINSIHED CGI
	else{ //GET STARTED WITH POLLING METHODS
		if (request->_method_type == GET)
			response = getMethod(request, response);
		else if (request->_method_type == POST)
			response = postMethod(request, response);
		else if (request->_method_type == DELETE)
			response = deleteMethod(request, response);
		response->setResponseBuffer(response->generateResponse()); //call in generateResponse
	}
	if (response->getResponseHandlerStatus() == responseHandlerStatus::READY_TO_WRITE){
		response->setResponseHandlerStatus(responseHandlerStatus::WRITING);
		size_t bytesWritten = 0;
		while (response->getBytesWritten() < response->getResponseBuffer().size()){
			bytesWritten = write(request->_clientFD, response->getResponseBuffer().c_str() + bytesWritten, BUFFER_SIZE);
			if (bytesWritten == -1){
				response->autoFillResponse("500 Internal Server Error: write");//is this ok?
				break;
			}
			response->setBytesWritten(bytesWritten);
		}
	}
	if (response->getBytesWritten() == response->getResponseBuffer().size())
		response->setResponseHandlerStatus(responseHandlerStatus::FINISHED);
	return;
}
