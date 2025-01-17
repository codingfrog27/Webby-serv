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
	reqRules = checkLocationMatch(this->_config->_locations, reqRules);
	// while nested locs exist //check for any match and if match uset setlocrules to overwrite
	
   
}

location	Request::checkLocationMatch(std::vector<location> &locs, location &ruleblock)
{ 
	// 1: Exact Match (=)
	// 2: Longest Prefix Match
	// 3: Regex Match
	// 4: Default Location
	size_t	bestMatchSize = 0;
	size_t	newSize;
	for (std::vector<location>::iterator it = locs.begin(); it != locs.end(); ++it)
	{
		newSize = countPathMatch(_filePath, it->getName());
		if (newSize == std::string::npos)
			return *it;
		if (newSize > bestMatchSize)
		{
			bestMatchSize = newSize;
			ruleblock = *it;
		}
	}
	return (ruleblock);
}

size_t	Request::countPathMatch(std::string &reqpath, std::string &locpath)
{
	size_t	size = 0;
	for (size < reqpath.size() && size < locpath.size() && reqpath[size] == locpath[size]; size++;)
	if (size == reqpath.size())
		return (std::string::npos);
	return (size);
}

void assignStrIfNonEmpty(std::string &dest, std::string &rhs)
{
	if (!rhs.empty())
		dest = rhs;
}

void	Request::setLocRules(location &loc, location &ruleblock)
{
	// assignStrIfNonEmpty(ruleblock._allow_methods, loc._allow_methods); //change after merge
	// assignStrIfNonEmpty(ruleblock._index, loc._index);
	assignStrIfNonEmpty(ruleblock._alias, loc._alias);
	assignStrIfNonEmpty(ruleblock._return, loc._return);
	assignStrIfNonEmpty(ruleblock._root, loc._root);
	if (!loc._index.empty())
		ruleblock._index = loc._index;
	if (!loc._cgi_extension.empty())
		ruleblock._cgi_extension = loc._cgi_extension;
	if (!loc._cgi_path.empty())
		ruleblock._cgi_path = loc._cgi_path;
	checkLocationMatch(loc._nestedLocations, ruleblock);
}


// checkLocRules(location &ruleblock)
// {

// }

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


