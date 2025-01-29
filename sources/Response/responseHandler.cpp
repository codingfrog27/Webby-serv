#include "Response.hpp"
#include "Request.hpp"
#include "CGI.hpp"
#include "libft.h"
#include "Connection.hpp"

void	Response::getMethod(Request* request, Response* response){
	size_t size = 0;

	if(_responseHandlerStatus == responseHandlerStatus::IN_PROGRESS){
		if (fileExists(request->_filePath)){
			setContentType(request->_filePath);
			if (getReadingModeFromResponse() == BINARY)
				getInFile().open(request->_filePath, std::ios::binary);
			else
				getInFile().open(request->_filePath);
			if (!getInFile().is_open()){
				autoFillResponse("500 Internal Server Error: GET");
				return ;
			}
		}
		else{
			autoFillResponse("404 Not Found");
			return ;
		}
		getInFile().seekg(0, std::ios::end);
		size = getInFile().tellg();
		if (size == 0){
			getInFile().close();
			autoFillResponse("204 No Content");
			return ;
		}
		getInFile().seekg(0, std::ios::beg);
		setHeaders("Content-Length", std::to_string(size));
		_responseHandlerStatus = responseHandlerStatus::IN_GET;
	}
	if (getInFile().is_open() && _responseHandlerStatus == responseHandlerStatus::IN_GET){
		std::unique_ptr<std::vector<char>> buffer = std::make_unique<std::vector<char>>(BUFFER_SIZE);
		getInFile().read(buffer->data(), BUFFER_SIZE);
		// if (getInFile().fail()){ // is triggered after second read
		// 	getInFile().close();
		// 	autoFillResponse("500 Internal Server Error: GET");
		// 	return ;
		// }
		setBody(std::string(buffer->begin(), buffer->end()));
		if (getInFile().eof()){
			getInFile().close();
			setStatus("200 OK");
			setResponseBuffer(generateResponse());
			_responseHandlerStatus = responseHandlerStatus::READY_TO_WRITE;
		}
	}
	else
		autoFillResponse("500 Internal Server Error: GET");
	return ;
}

void	Response::postMethod(Request* request, Response* response){
	// check for CGI??
	if (_responseHandlerStatus == responseHandlerStatus::IN_PROGRESS){
		if(getReadingModeFromRequest(*request) == BINARY)
			getOutFile().open(request->_filePath, std::ios::binary);
		else
			getOutFile().open(request->_filePath);
		if (!getOutFile().is_open()){
			autoFillResponse("500 Internal Server Error: POST");
			return ;
		}
		_responseHandlerStatus = responseHandlerStatus::IN_POST;
	}
	if (getOutFile().is_open() && _responseHandlerStatus == responseHandlerStatus::IN_POST){
		getOutFile().write(request->getBody().c_str() + getBytesWritten(), BUFFER_SIZE);
		if (getOutFile().fail()){
			autoFillResponse("500 Internal Server Error: POST");
			getOutFile().close();
			return ;
		}
		setBytesWritten(BUFFER_SIZE);
		if (getBytesWritten() >= request->getBody().size()){
			getOutFile().close();
			autoFillResponse("201 Created");
			setResponseBuffer(generateResponse());
			setBytesWritten(0);
			_responseHandlerStatus = responseHandlerStatus::READY_TO_WRITE;
		}
		return ;
	}
	else
		autoFillResponse("500 Internal Server Error");
	return ;
}

void	Response::deleteMethod(Request* request, Response* response){
	if(_responseHandlerStatus == responseHandlerStatus::IN_PROGRESS)
		_responseHandlerStatus = responseHandlerStatus::IN_DELETE;
	if (fileExists(request->_filePath)){
		if (std::remove(request->_filePath.c_str()) == 0)
			autoFillResponse("200 OK");
		else
			autoFillResponse("500 Internal Server Error");
	}
	else
		autoFillResponse("404 Not Found");
	return ;
}

//config for timeout & max body size
connectStatus	Response::responseHandler(Request* request, Response* response){
	if (_responseHandlerStatus == responseHandlerStatus::NOT_STARTED){
		_responseHandlerStatus = responseHandlerStatus::IN_PROGRESS;
		setHTTPVersion(request->_http_version);
		if (request->_headers.find("Connection") != request->_headers.end() && request->_headers["Connection"] == "close")
			setHeaders("Connection", "close");
		else
			setHeaders("Connection", "keep-alive");
	}
	if (_responseHandlerStatus == responseHandlerStatus::IN_PROGRESS && !request->getStatusCode().empty()){ //if there was an error in (parsing) the request{}
		autoFillResponse(request->getStatusCode());
		return connectStatus::RESPONDING;
	}
	// std::cout << MAGENTA "Method		: " << request->_method_type << " (0 = GET, 1 = POST, 2 = DELETE)" RESET << std::endl;
	// std::cout << MAGENTA "Content-type	: " << request->getHeaderValue("Content-Type") << RESET << std::endl;
	// std::cout << MAGENTA "filepath	: " << request->_filePath << RESET << std::endl;
	// if (_responseHandlerStatus == responseHandlerStatus::IN_CGI || (_responseHandlerStatus == responseHandlerStatus::IN_PROGRESS && isCGIrequired(request))){
	// 	CGIHandler(request, response); //FINSIHED CGI
	// 	return connectStatus::RESPONDING;
	// }
	else{
		if ((request->_method_type == GET && _responseHandlerStatus == responseHandlerStatus::IN_PROGRESS) || _responseHandlerStatus == responseHandlerStatus::IN_GET){
			getMethod(request, response);
			return connectStatus::RESPONDING;
		}
		else if ((request->_method_type == POST && _responseHandlerStatus == responseHandlerStatus::IN_PROGRESS) || _responseHandlerStatus == responseHandlerStatus::IN_POST){
			postMethod(request, response);
			return connectStatus::RESPONDING;
		}
		else if ((request->_method_type == DELETE && _responseHandlerStatus == responseHandlerStatus::IN_PROGRESS) || _responseHandlerStatus == responseHandlerStatus::IN_DELETE) {
			deleteMethod(request, response);
			return connectStatus::RESPONDING;
		 }
	}
	if (_responseHandlerStatus == responseHandlerStatus::READY_TO_WRITE || _responseHandlerStatus == responseHandlerStatus::WRITING) {
		_responseHandlerStatus = responseHandlerStatus::WRITING;
		return (writeResponse(request->_clientFD));
	}
	return connectStatus::RESPONDING;
}
