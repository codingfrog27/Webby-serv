#include "Response.hpp"
#include "Request.hpp"
#include "Connection.hpp"

Response::Response(Config *config) {
	this->_responseHandlerStatus = responseHandlerStatus::NOT_STARTED;
	this->_responseBuffer = "";
	this->_bytesWritten = 0;
	this->_timesWriteFailed = 0;
	this->setHTTPVersion("HTTP/1.1");
	setHeaders("Root", config->_rootDir);
	return ;
}

Response::Response(const Response &obj){
	*this = obj;
}

Response &	Response::operator=(const Response &rhs)
{
	std::cout << GREEN << "Response: Assignment operator called" << RESET << std::endl;

	if (this != &rhs)
	{
		_httpVersion = rhs._httpVersion;
		_responseHandlerStatus = rhs._responseHandlerStatus;
		_status = rhs._status;
		_headers = rhs._headers;
		_body = rhs._body;
		_responseBuffer = rhs._responseBuffer;
		_bytesWritten = rhs._bytesWritten;
		_timesWriteFailed = rhs._timesWriteFailed;
	}

	return (*this);
}

Response::~Response(){
	return ;
}

void	Response::autoFillResponse(std::string status){
	std::string		path = _headers["Root"] + "/error_pages/" + status.substr(0, 3) + ".html";
	// std::cout << YELLOW "Root in autofill: " RESET << _headers["Root"] << std::endl;
	// std::cout << YELLOW "path: " RESET << path << std::endl;
	size_t			size = 0;
	std::ifstream	file(path);

	Response::setStatus(status);
	if (!_body.empty())
		_body.clear();
	if (file.is_open()){
		// std::cout << YELLOW "file is open" RESET << std::endl;
		file.seekg(0, std::ios::end);
		size = file.tellg();
		file.seekg(0, std::ios::beg);
		std::vector<char> buffer(size + 1);
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
		// std::cout << YELLOW "file is not open" RESET << std::endl;
		Response::setHeaders("Content-Type", "text/plain");
		Response::setHeaders("Content-Length", std::to_string(status.length()));
		Response::setBody(status);
	}
	Response::setResponseBuffer(Response::generateResponse());
	_responseHandlerStatus = responseHandlerStatus::READY_TO_WRITE;
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
	std::string response = _httpVersion + " " + _status + "\r\n";
	// std::cout << YELLOW "HTTP version: " << _httpVersion << "\nStatus: " << _status << RESET << std::endl;
	for (auto it = _headers.begin(); it != _headers.end(); it++){
		response += it->first + ": " + it->second + "\r\n";
	}
	response += "\r\n";
	response += std::string(_body.begin(), _body.end());

	return response;
}

connectStatus Response::writeResponse(int FD){
	size_t n =_responseBuffer.size() - _bytesWritten;
	// std::cout << YELLOW "\nresponse buffer size: " << _responseBuffer.size() << "\n bytes written: " << _bytesWritten << "\n n: " << n << RESET << std::endl;
	if (n > BUFFER_SIZE)
		n = BUFFER_SIZE;
	// std::cout << "writing " << n << " bytes" << std::endl;
	size_t bytes = send(FD, _responseBuffer.c_str() + _bytesWritten, n, 0); 
	// write(STDOUT_FILENO, _responseBuffer.c_str() + _bytesWritten, n);
	// std::ofstream outFile("Response written.txt", std::ios::app);
	// outFile << _responseBuffer.substr(_bytesWritten, bytes)  << std::endl;
	_bytesWritten += bytes;
	if (_bytesWritten >= _responseBuffer.size()){
		setResponseHandlerStatus(responseHandlerStatus::FINISHED);
		return connectStatus::FINISHED;
	}
	if (bytes < 0){
		if (_timesWriteFailed == 2){
			setResponseHandlerStatus(responseHandlerStatus::FINISHED);
			return connectStatus::FINISHED;
		}
		autoFillResponse("500 Internal Server Error: write");//is this ok?
		_timesWriteFailed++;
		return connectStatus::RESPONDING;
	}

	return connectStatus::RESPONDING;
}

/*	Setters	*/

void	Response::setHTTPVersion(std::string HTTPversion){
	_httpVersion = HTTPversion;
	return ;
}

void	Response::setResponseHandlerStatus(responseHandlerStatus status){
	_responseHandlerStatus = status;
	return ;
}

// void	Response::setCGI(CGI* cgi){
// 	_cgi = cgi;
// 	return ;
// }

// void	Response::setOutFile(std::ofstream&& outFile){
// 	if (_outFile.is_open())
// 		_outFile.close();
// 	_outFile = std::move(outFile);
// 	return ;
// }

// void	Response::setInFile(std::ifstream&& inFile){
// 	if (_inFile.is_open())
// 		_inFile.close();
// 	_inFile = std::move(inFile);
// 	return ;
// }

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
	if (_body.empty())
		_body = body;
	else
		_body += body;
	return ;
}

void	Response::setBody(std::vector<char> body){
	if (_body.empty())
		_body.assign(body.begin(), body.end());
	else
		_body.insert(_body.end(), body.begin(), body.end());
	return ;
}

void	Response::setResponseBuffer(std::string buffer){
	_responseBuffer += buffer;
	// std::cout << "Response buffer: " << _responseBuffer << std::endl;
	return ;
}

void	Response::setBytesWritten(size_t bytesWritten){
	if (bytesWritten == 0)
		_bytesWritten = bytesWritten;
	else
		_bytesWritten += bytesWritten;
	return ;
}

/*	getters	*/

responseHandlerStatus	Response::getResponseHandlerStatus() const{
	return _responseHandlerStatus;
}

// CGI*	Response::getCGI() const{
// 	return _cgi;
// }

std::ofstream&	Response::getOutFile(){
	// if (!_outFile.is_open())
	// 	throw std::runtime_error("Outfile not set");
	return _outFile;
}

std::ifstream&	Response::getInFile(){
	// if (!_inFile.is_open())
	// 	throw std::runtime_error("Infile not set");
	return _inFile;
}

/* anything that's not text has to be handled as raw bits*/
readingMode	Response::getReadingModeFromResponse() const{
	if (Response::getHeader("Content-type").find("text") != std::string::npos)
		return TEXT;
	else
		return BINARY;
}

readingMode	Response::getReadingModeFromRequest(Request & request) const{
	if (request._headers["Content-Type"].find("text") != std::string::npos)
		return TEXT;
	else
		return BINARY;
}

/*	takes a string as key, finds it in the map and returns the value	*/
std::string	Response::getHeader(std::string key) const{
	auto it = _headers.find(key);
	if (it == _headers.end())
		return ("");
	return it->second;
}

std::string	Response::getResponseBuffer() const{
	return _responseBuffer;
}

size_t	Response::getBytesWritten() const{
	return _bytesWritten;
}
