#include "Response.hpp"
#include "Request.hpp"
#include "CGI.hpp"

static void	getMethod(Request* request, Response* response){
	size_t size = 0;

	if(response->getResponseHandlerStatus() == responseHandlerStatus::IN_PROGRESS){
		if (fileExists(request->_filePath)){
			response->setContentType(request->_filePath);
			if (response->getReadingModeFromResponse() == BINARY)
				response->getInFile().open(request->_filePath, std::ios::binary);
			else
				response->getInFile().open(request->_filePath);
			if (!response->getInFile().is_open()){
				response->autoFillResponse("500 Internal Server Error: GET");
				return ;
			}
		}
		else{
			response->autoFillResponse("404 Not Found");
			return ;
		}
		response->getInFile().seekg(0, std::ios::end);
		size = response->getInFile().tellg();
		if (size == 0){
			response->getInFile().close();
			response->autoFillResponse("204 No Content");
			return ;
		}
		response->getInFile().seekg(0, std::ios::beg);
		response->setHeaders("Content-Length", std::to_string(size));
		response->setResponseHandlerStatus(responseHandlerStatus::IN_GET);
	}
	if (response->getInFile().is_open() && response->getResponseHandlerStatus() == responseHandlerStatus::IN_GET){
		std::unique_ptr<std::vector<char>> buffer = std::make_unique<std::vector<char>>(BUFFER_SIZE);
		response->getInFile().read(buffer->data(), BUFFER_SIZE);
		if (response->getInFile().fail()){
			response->getInFile().close();
			response->autoFillResponse("500 Internal Server Error: GET");
			return ;
		}
		response->setBody(std::string(buffer->begin(), buffer->end()));
		if (response->getInFile().eof()){
			response->getInFile().close();
			response->setStatus("200 OK");
			response->setResponseHandlerStatus(responseHandlerStatus::READY_TO_WRITE);
		}
	}
	else
		response->autoFillResponse("500 Internal Server Error: GET");
	return ;
}

static void	postMethod(Request* request, Response* response){
	// check for CGI??
	if (response->getResponseHandlerStatus() == responseHandlerStatus::IN_PROGRESS){
		if(response->getReadingModeFromRequest(*request) == BINARY)
			response->getOutFile().open(request->_filePath, std::ios::binary);
		else
			response->getOutFile().open(request->_filePath);
		if (!response->getOutFile().is_open()){
			response->autoFillResponse("500 Internal Server Error: POST");
			return ;
		}
		response->setResponseHandlerStatus(responseHandlerStatus::IN_POST);
	}
	if (response->getOutFile().is_open() && response->getResponseHandlerStatus() == responseHandlerStatus::IN_POST){
		response->getOutFile().write(request->getBody().c_str() + response->getBytesWritten(), BUFFER_SIZE);
		if (response->getOutFile().fail()){
			response->autoFillResponse("500 Internal Server Error: POST");
			response->getOutFile().close();
			return ;
		}
		response->setBytesWritten(BUFFER_SIZE);
		if (response->getBytesWritten() >= request->getBody().size()){
			response->getOutFile().close();
			response->autoFillResponse("201 Created");
			response->setBytesWritten(0);
			response->setResponseHandlerStatus(responseHandlerStatus::READY_TO_WRITE);
		}
		return ;
	}
	else
		response->autoFillResponse("500 Internal Server Error");
	return ;
}

static void	deleteMethod(Request* request, Response* response){
	if(response->getResponseHandlerStatus() == responseHandlerStatus::IN_PROGRESS)
		response->setResponseHandlerStatus(responseHandlerStatus::IN_DELETE);
	if (fileExists(request->_filePath)){
		if (std::remove(request->_filePath.c_str()) == 0)
			response->autoFillResponse("200 OK");
		else
			response->autoFillResponse("500 Internal Server Error");
	}
	else
		response->autoFillResponse("404 Not Found");
	return ;
}

//config for timeout & max body size
void	responseHandler(Request* request, Response* response, Config* config){
	if (response->getResponseHandlerStatus() == responseHandlerStatus::NOT_STARTED){
		response->setResponseHandlerStatus(responseHandlerStatus::IN_PROGRESS);
		response->setHTTPVersion(request->_http_version);
	}
	(void)config;
	if (response->getResponseHandlerStatus() == responseHandlerStatus::IN_PROGRESS && !request->getStatusCode().empty()){ //if there was an error in (parsing) the request{}
		response->autoFillResponse(request->getStatusCode());
		return ;
	}
	// std::cout << MAGENTA "Method		: " << request->_method_type << " (0 = GET, 1 = POST, 2 = DELETE)" RESET << std::endl;
	// std::cout << MAGENTA "Content-type	: " << request->getHeaderValue("Content-Type") << RESET << std::endl;
	// std::cout << MAGENTA "filepath	: " << request->_filePath << RESET << std::endl;
	if (response->getResponseHandlerStatus() == responseHandlerStatus::IN_CGI || isCGIrequired(request)){
		CGIHandler(request, response); //FINSIHED CGI
		return ;
	}
	else{
		if ((request->_method_type == GET && response->getResponseHandlerStatus() == responseHandlerStatus::IN_PROGRESS) || response->getResponseHandlerStatus() == responseHandlerStatus::IN_GET){
			getMethod(request, response);
			return ;
		}
		else if ((request->_method_type == POST && response->getResponseHandlerStatus() == responseHandlerStatus::IN_PROGRESS) || response->getResponseHandlerStatus() == responseHandlerStatus::IN_POST){
			postMethod(request, response);
			return ;
		}
		else if ((request->_method_type == DELETE && response->getResponseHandlerStatus() == responseHandlerStatus::IN_PROGRESS) || response->getResponseHandlerStatus() == responseHandlerStatus::IN_DELETE)
			deleteMethod(request, response);
	}
	if (response->getResponseHandlerStatus() == responseHandlerStatus::READY_TO_WRITE || response->getResponseHandlerStatus() == responseHandlerStatus::WRITING){
		response->setResponseHandlerStatus(responseHandlerStatus::WRITING);
		int bytes = write(request->_clientFD, response->getResponseBuffer().c_str() + response->getBytesWritten(), BUFFER_SIZE); //send instead of write maybe? look at error handling
		if (bytes == -1){
			response->autoFillResponse("500 Internal Server Error: write");//is this ok?
			return ;
		}
		response->setBytesWritten(bytes);
		if (response->getBytesWritten() == response->getResponseBuffer().size()){
			response->setBytesWritten(0);
			response->setResponseHandlerStatus(responseHandlerStatus::FINISHED);
		}
	}
	return;
}
