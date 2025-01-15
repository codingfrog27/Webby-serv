#include "everything.hpp"
#include <filesystem>
#include "socket.hpp"

std::string generate_directory_listing(const std::string& _filePath) //should become response func 
{
	std::ostringstream html;

	html << "<html><head><title>Directory Listing</title></head><body>\n";
	html << "<h1>Directory listing for " << _filePath << "</h1>\n";
	html << "<ul>\n";

	if (std::filesystem::exists(_filePath) && std::filesystem::is_directory(_filePath))
	{
		for (const auto& it : std::filesystem::directory_iterator(_filePath)) 
		{
			std::string entry_name = it.path().filename().string();
			if (it.is_directory()) 
				html << "<li><a href='" << entry_name << "/'>" << entry_name << "/</a></li>\n";
			else 
				html << "<li><a href='" << entry_name << "'>" << entry_name << "</a></li>\n";
		}
	}

	html << "</ul>\n";
	html << "</body></html>\n";

	return html.str();
}

void	create_directory_listing_page(std::string html_page)
{
	std::fstream	File;

	File.open("directory_listing.html", std::ios::out);
	File << html_page;
	File.close();
}

void sendHTMLPage(int client_socket, const std::string& file_path) 
{
	//Open the HTML file
	std::ifstream file(file_path);
	if (!file) 
	{
		std::cout << RED << "Error opening file: " << file_path << RESET << std::endl;
		return;
	}

	//Read the file content, store it in buffer and covert it into a string
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string html_content = buffer.str();

	//HTTP Response Headers
	std::string http_response = "HTTP/1.1 200 OK\r\n";
	http_response += "Content-Type: text/html\r\n";
	http_response += "Content-Length: " + std::to_string(html_content.size()) + "\r\n";
	http_response += "Connection: close\r\n";
	http_response += "\r\n";

	//Send the HTTP header
	send(client_socket, http_response.c_str(), http_response.size(), 0);
	//Send the client file content	  /// add html_content to http_response!!!!!!!
	send(client_socket, html_content.c_str(), html_content.size(), 0);
	std::cout << YELLOW << "--------- HTML message sent ----------" << RESET << std::endl;

	file.close();
}

void	Request::checkLocations(std::string _filePath)
{
	location	reqRules;
	std::cout << "current req _filePath == (b4 loc-check) " << _filePath << std::endl \
	<< "FD == " << _clientFD << std::endl;
	checkLocationMatch(this->_config->_locations, reqRules);

		

   
}
void	Request::checkLocationMatch(std::vector<location> &locs, location &ruleblock)
{ 
	for (size_t i = 0; i < locs.size(); i++)
	{
		if (_filePath.find(locs[i].getRoot()))
			setLocRules(locs[i], ruleblock);
	}
	
}

void	Request::setLocRules(location &loc, location &ruleblock)
{
	if ()
	ruleblock.
	checkLocationMatch(loc._nestedLocations, ruleblock);

}

checkLocRules(location &ruleblock)
{

}

// void checkRules(location &rules)
// {	//loop through allowed methods to check if is allowed
// 	for (size_t i = 0; i < count; i++)
// 	{
// 		/* code */
// 	}
// 	//if redirect change path to redirect path
// 	//if autoindex && filepath == folder change path to index
// 	//alias?? maybe cgi??
// }


