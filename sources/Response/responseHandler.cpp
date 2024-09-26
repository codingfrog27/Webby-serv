#include "Response.hpp"
#include <fstream>

// decide repsonse based on request

/* extract Filepath from URI*/
std::string resolveFilePath(HttpRequest* request){
	std::string uri = request->_URI;
	std::string host = request->_headers["Host"];
	std::string file = uri.erase(0, host.length()); ///host length + 8 if http:// or 9 if https:// with request target in absolute form
	std::string path = request->_headers["Root"] + file;
	return path;
}

/* check file exists*/
bool fileExists(std::string path, Response* response){
	std::ifstream file(path);
	if (file.is_open()){
		file.close();
		response->setStatus("200 OK");
		return true;
	}
	else{
		response->setStatus("404 Not Found");
		return false;
	}
	return;
}

// Response::generateResponse(httpRequest);

Response* responseHandler(HttpRequest* request)
{
	Response Response = Response(request);
	std::string path = resolveFilePath(request);
	if (fileExists(path, &Response)){
		std::ifstream file(path);
		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line)) // best way for images?
			{
				Response.setBody() //+= line;
			}
			file.close();
		}
	}
	else
	{
		// Response = "HTTP/1.1 404 Not Found\r\n";
		// Response += "Content-Type: text/html\r\n";
		// Response += "\r\n";
		// Response += "<html><body><h1>404 Not Found</h1></body></html>";
	}
	return &Response;
}
