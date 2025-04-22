#include "Response.hpp"
#include "Request.hpp"
#include "CGI.hpp"
#include "libft.h"
#include "Connection.hpp"

void	Response::getMethod(Request* request){
	size_t size = 0;

	if(_responseHandlerStatus == responseHandlerStatus::IN_PROGRESS){
		if (request->_dirListing == true){
			std::string dirListing = generateDirectoryListing(request->_filePath);
			setHeaders("Content-Type", "text/html");
			setHeaders("Content-Length", std::to_string(dirListing.size()));
			setBody(dirListing);
			setStatus("200 OK");
			setResponseBuffer(generateResponse());
			_responseHandlerStatus = responseHandlerStatus::READY_TO_WRITE;
			return ;
		}
		if (fileExists(request->_filePath)){
			setContentType(request->_filePath);
			if (getReadingModeFromResponse() == BINARY)
				getInFile().open(request->_filePath, std::ios::binary);
			else
				getInFile().open(request->_filePath);
			if (!getInFile().is_open()){
				request->_statusCode = 500;
				request->_statusStr = "500 Internal Server Error";
				return ;
			}
		}
		else{
			request->_statusCode = 404;
			request ->_statusStr = "404 Not Found";
			return ;
		}
		getInFile().seekg(0, std::ios::end);
		size = getInFile().tellg();
		if (size == 0){
			getInFile().close();
			autoFillResponse("204 No Content", "");
			return ;
		}
		getInFile().seekg(0, std::ios::beg);
		setHeaders("Content-Length", std::to_string(size));
		_responseHandlerStatus = responseHandlerStatus::IN_GET;
	}
	if (getInFile().is_open() && _responseHandlerStatus == responseHandlerStatus::IN_GET){
		std::unique_ptr<std::vector<char>> buffer = std::make_unique<std::vector<char>>(BUFFER_SIZE);
		getInFile().read(buffer->data(), BUFFER_SIZE);
		setBody(std::string(buffer->begin(), buffer->end()));
		if (getInFile().eof()){
			getInFile().close();
			setStatus("200 OK");
			setResponseBuffer(generateResponse());
			_responseHandlerStatus = responseHandlerStatus::READY_TO_WRITE;
		}
	}
	else{
		request->_statusCode = 500;
		request->_statusStr = "500 Internal Server Error";
		_responseHandlerStatus = responseHandlerStatus::IN_PROGRESS;
	}
	return ;
}

void	Response::postMethod(Request* request){
	if (_responseHandlerStatus == responseHandlerStatus::IN_PROGRESS){
		if(getReadingModeFromRequest(*request) == BINARY)
			getOutFile().open(request->_filePath, std::ios::binary);
		else
			getOutFile().open(request->_filePath);
		if (!getOutFile().is_open()){
			request->_statusCode = 500;
			request->_statusStr = "500 Internal Server Error";
			return ;
		}
		_responseHandlerStatus = responseHandlerStatus::IN_POST;
	}
	if (getOutFile().is_open() && _responseHandlerStatus == responseHandlerStatus::IN_POST){
		getOutFile().write(request->getBody().c_str() + getBytesWritten(), BUFFER_SIZE);
		if (getOutFile().fail()){
			request->_statusCode = 500;
			request->_statusStr = "500 Internal Server Error";
			_responseHandlerStatus = responseHandlerStatus::IN_PROGRESS;
			getOutFile().close();
			return ;
		}
		setBytesWritten(BUFFER_SIZE);
		if (getBytesWritten() >= request->getBody().size()){
			getOutFile().close();
			autoFillResponse("201 Created", "");
			setBytesWritten(0);
			_responseHandlerStatus = responseHandlerStatus::READY_TO_WRITE;
		}
		return ;
	}
	else{
		request->_statusCode = 500;
		request->_statusStr = "500 Internal Server Error";
		_responseHandlerStatus = responseHandlerStatus::IN_PROGRESS;
	}
	return ;
}

void	Response::deleteMethod(Request* request){
	if(_responseHandlerStatus == responseHandlerStatus::IN_PROGRESS)
		_responseHandlerStatus = responseHandlerStatus::IN_DELETE;
	if (fileExists(request->_filePath)){
		if (std::remove(request->_filePath.c_str()) == 0)
			autoFillResponse("200 OK", "");
		else{
			request->_statusCode = 500;
			request->_statusStr = "500 Internal Server Error";
			_responseHandlerStatus = responseHandlerStatus::IN_PROGRESS;
		}
	}
	else{
		request->_statusCode = 404;
		request ->_statusStr = "404 Not Found";
		_responseHandlerStatus = responseHandlerStatus::IN_PROGRESS;
	}
	return ;
}

connectStatus	Response::responseHandler(Request* request){
	if (_responseHandlerStatus == responseHandlerStatus::NOT_STARTED){
		_responseHandlerStatus = responseHandlerStatus::IN_PROGRESS;
		if (request->_headers.find("Connection") != request->_headers.end() && request->_headers["Connection"] == "close")
			setHeaders("Connection", "close");
		else
			setHeaders("Connection", "keep-alive");
	}
	if (_responseHandlerStatus == responseHandlerStatus::IN_PROGRESS && request->_statusCode != 0) //if there was an error in (parsing) the request{}
	{ 
		if (request->_statusCode == 301 || request->_statusCode == 302){
			setHeaders("Location", request->_filePath);
			autoFillResponse(request->_statusStr, "");
			return connectStatus::RESPONDING;
		}
		if (request->getConfig()->_errorPage.find(std::to_string(request->_statusCode)) != request->getConfig()->_errorPage.end())
		{
			request->_filePath = request->getConfig()->_errorPage[std::to_string(request->_statusCode)];
			if (request->_filePath == "default")
			{
				request->_filePath = _root + "/cgi-bin/error.js";
				request->_method_type = Http_method::GET;
				return connectStatus::CGI_REQUIRED;
			}
			else{
				autoFillResponse(request->_statusStr, request->_filePath);
				return connectStatus::RESPONDING;
			}
		}
		else{
			request->_filePath = _root + "/cgi-bin/error.js";
			request->_method_type = Http_method::GET;
			return connectStatus::CGI_REQUIRED;
		}
	}
	else
	{
		if ((request->_method_type ==  Http_method::GET && _responseHandlerStatus == responseHandlerStatus::IN_PROGRESS) || _responseHandlerStatus == responseHandlerStatus::IN_GET){
			getMethod(request);
			return connectStatus::RESPONDING;
		}
		else if ((request->_method_type == Http_method::POST && _responseHandlerStatus == responseHandlerStatus::IN_PROGRESS) || _responseHandlerStatus == responseHandlerStatus::IN_POST){
			postMethod(request);
			return connectStatus::RESPONDING;
		}
		else if ((request->_method_type == Http_method::DELETE && _responseHandlerStatus == responseHandlerStatus::IN_PROGRESS) || _responseHandlerStatus == responseHandlerStatus::IN_DELETE) {
			deleteMethod(request);
			return connectStatus::RESPONDING;
		 }
	}
	if (_responseHandlerStatus == responseHandlerStatus::READY_TO_WRITE || _responseHandlerStatus == responseHandlerStatus::WRITING) {
		_responseHandlerStatus = responseHandlerStatus::WRITING;
		return (writeResponse(request->_clientFD));
	}
	return connectStatus::RESPONDING;
}
