/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   HttpRequest_parsing.cpp							:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/09/24 14:11:35 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/04 18:04:40 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Request.hpp"

// not trimming any trailing whitespace rn cause RFC 7230 states its not allowed
// some irl servers do allow it though.. so might add later?
void	Request::parse_headers(std::string header_str)
{
	size_t						start, colon_pos, line_end;
	std::string					key, value;

	start = parse_req_line(header_str);
	line_end = header_str.find("\r\n", start);
	while (line_end != std::string::npos)
	{
		colon_pos = header_str.find(':', start);
		if (colon_pos == std::string::npos)
			throw (std::invalid_argument("colon missing in header"));
		key = header_str.substr(start, colon_pos - start);
		value = header_str.substr(colon_pos + 2, line_end - (colon_pos + 2)); //check for empty values?
		_headers[key] = value;
		start = line_end + 2;
		line_end = header_str.find("\r\n", start);
	}
	checkHeaders();
}

size_t	Request::parse_req_line(std::string req_line)
{
	size_t	line_end, method_end, uri_end;
	
	line_end = req_line.find("\r\n");
	if (line_end == 0)
	{
		while (req_line[line_end] == '\r' || req_line[line_end] == '\n')
			line_end++;
		line_end = req_line.find("\r\n");
	}
	if (line_end == std::string::npos)
		throw (std::invalid_argument("no CRLF found"));

	method_end = req_line.find(' ');
	if (method_end == std::string::npos)
		throw (std::invalid_argument("no space found after method"));
	uri_end = req_line.find(' ', method_end + 1); //could check on single space
	if (uri_end == std::string::npos)
		throw (std::invalid_argument("no space found after uri"));

	_method_type = which_method_type(req_line.substr(0, method_end));
	_URI = req_line.substr(method_end + 1, uri_end - method_end - 1);
	_http_version = http_version(&req_line[uri_end + 1]);
	return (line_end + 2);
}

Http_method Request::which_method_type(std::string str) 
{
	const char *Methods[] = {"GET", "POST", "DELETE"}; //will be updated after config parsing

	for (size_t i = 0; i < 3; i++)
	{
		if (str == Methods[i])
			return (static_cast<Http_method>(i));
	}
	throw std::invalid_argument("Unsupported HTTP method: " + str);
}


void Request::dechunkBody() //from copilot will check next week
{
// 	// https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Transfer-Encoding
// 	  // Implementation for dechunking the body
//	 std::vector<char> body = _rawRequestData; // Assuming you have a method to get the body content as a vector of chars
//	 std::vector<char> dechunked_body;
//	 size_t pos = 0;

//	 while (pos < body.size()) {
//		 auto chunk_size_end = std::search(body.begin() + pos, body.end(), "\r\n", "\r\n" + 2);
//		 if (chunk_size_end == body.end()) {
//			 throw std::invalid_argument("400 Bad Request: Invalid chunked encoding");
//		 }

//		 std::string chunk_size_str(body.begin() + pos, chunk_size_end);
//		 size_t chunk_size = std::stoul(chunk_size_str, nullptr, 16);
//		 pos = chunk_size_end - body.begin() + 2;

//		 if (chunk_size == 0) {
//			 break; // End of chunks
//		 }

//		 if (pos + chunk_size > body.size()) {
//			 throw std::invalid_argument("400 Bad Request: Incomplete chunked encoding");
//		 }

//		 dechunked_body.insert(dechunked_body.end(), body.begin() + pos, body.begin() + pos + chunk_size);
//		 pos += chunk_size + 2; // Skip chunk data and CRLF
//	 }

	
// 	//think maybe i just stoul() the size given (tho its hex should remember)
// 	// and then throw the data inbetween in a new buffer?
}

float Request::http_version(std::string version) //throw error if not 1 or 1.1
{
	if (version.compare(0, 6, "HTTP/1") != 0 && version.compare(0, 8, "HTTP/1.1") != 0)
		throw std::invalid_argument("Unsupported HTTP version: " + version);
	return (std::stof(version.substr(5))); 
}

void	Request::parseBody()
{
	const std::string	suffix = "--";
		std::string		content_type = getHeaderValue("Content-Type"); //make ref?
		std::string		delimiter;
		//assuming its there cause of header check
	
		
	if (getHeaderValue("transfer encoding") == "chunked")
		dechunkBody();
	if(content_type.compare("multipart/form-data; boundary=") == 0)
	{
		if (content_type.size() < 31)
			throw(std::invalid_argument("400, Bad Request, empty boundary parameter"));
		delimiter = content_type.substr(31);
	}
	


}
	// If any transfer coding other than chunked is applied to a request's
	//  content, the sender MUST apply chunked as the final transfer coding 
	//  to ensure that the message is properly framed. 
	// If any transfer coding other
	//  than chunked is applied to a response's content, the sender MUST either
	//   apply chunked as the final transfer coding or terminate the message by closing the connection.


//content encoding bestaat ook nog
//check for content length == body bytes read
	
//multipart/form data

void	Request::checkHeaders()
{
	// static const char *required_headers[] = {"Host", "connection"};
	// static const char	*required_for_post[] = {"Content-Lenth", "Content-Type"};
	//required headers
	if (!headerExists("Host")) //split?
		throw(std::invalid_argument("400 bad request: Host missing"));
	if (getHeaderValue("Connection") == "close")
		_keepOpen = false;
	// else if (getHeaderValue("Connection") == "keep-alive")
	// 	_keepOpen = true;
	// else
	// 	throw(std::invalid_argument("400 bad request: Conection missing"));
	if (_bodyFound)
	{
		if (_method_type == GET )
			throw (std::invalid_argument("GET requests should not have a body"));
		if (!headerExists("Content-Type")) //or if type not supported
			throw (std::invalid_argument("415 Bad request. Unsupported Media Type"));
		if (!headerExists("content-length") && getHeaderValue("Transfer-Encoding") != "chunked")
			throw (std::invalid_argument("411 length required"));
	}


}

//not sure but Authorization if thats in scope for protected resources
//accept
//not mandated but could return 406 Not Acceptable
// look into expext, mb range,
//  if-modified-since/if-none-match 
// ^ (could contain misuse but else return 304) 