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

// void	create_directory_listing_page(std::string html_page)
// {
// 	std::fstream	File;

// 	File.open("directory_listing.html", std::ios::out);
// 	File << html_page;
// 	File.close();
// }

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

// void	Request::locationHandler()
// {
// 	location	*reqRules;
// 	location	*nestRules;
// 	size_t		matchCount = 0;
// 	std::vector<location> &locVec = this->_config->_locations;
// 	// std::cout << "current req _filePath == (b4 loc-check) " << _filePath << std::endl \
// 	// << "FD == " << _clientFD << std::endl;
// 	if (locVec.empty())
// 		return;
// 	reqRules = findLocationMatch(locVec, matchCount);
// 	if (reqRules == nullptr)
// 		return;
// 	std::vector<location> &nestVec = reqRules->_nestedLocations;
// 	while (!nestVec.empty())
// 	{
// 		nestRules = findLocationMatch(nestVec, matchCount);
// 		if (nestRules == nullptr)
// 			break;
// 		setLocRules(*reqRules, *nestRules);
// 		nestVec = nestRules->_nestedLocations;
// 	}
// 		checkRules(*reqRules);
// }

void	Request::locationHandler()
{
	location	*locPtr;
	location	reqRules;
	size_t		matchCount = 0;
	std::vector<location> *locVec = &this->_config->_locations;
	if (locVec->empty())
		return;
	locPtr = findLocationMatch(*locVec, matchCount);
	if (locPtr == nullptr)
		return;
	reqRules = *locPtr;
	for (; !locVec->empty(); locVec = &locPtr->_nestedLocations)
	{
		locPtr = findLocationMatch(*locVec, matchCount);
		if (locPtr == nullptr)
			break;
		else
		setLocRules(reqRules, *locPtr);
	}
	checkRules(reqRules);
}

location	*Request::findLocationMatch(std::vector<location> &locs, size_t &matchCount)
{
	size_t	newSize;
	location *ret;
	bool	matchFound = false;
	for (std::vector<location>::iterator it = locs.begin(); it != locs.end(); ++it)
	{
		newSize = countPathMatch(_filePath, it->getName());
		if (newSize > matchCount) //?
		{
			matchCount = newSize;
			matchFound = true;
			ret = &(*it);
			if (newSize == std::string::npos)
				break;
		}
	}
	if (matchFound)
		return (ret);
	return (nullptr);
}

size_t	Request::countPathMatch(std::string &reqpath, std::string &locpath)
{
	size_t	size = 0;
	// while (locpath.at(size) == reqpath.at(size))
	for (;size < reqpath.size() && size < locpath.size()\
	 && reqpath[size] == locpath[size]; size++)
	if (size == locpath.size() && size == reqpath.size())
		return (std::string::npos);
		
	return (size);
}

void assignStrIfNonEmpty(std::string &dest, std::string &rhs)
{
	if (!rhs.empty())
		dest = rhs;
}

void	Request::setLocRules(location &ruleblock, location &loc)
{
	// assignStrIfNonEmpty(ruleblock._allow_methods, loc._allow_methods); //change after merge
	// assignStrIfNonEmpty(ruleblock._index, loc._index);
	assignStrIfNonEmpty(ruleblock._alias, loc._alias);
	assignStrIfNonEmpty(ruleblock._name, loc._name);
	assignStrIfNonEmpty(ruleblock._return, loc._return);
	assignStrIfNonEmpty(ruleblock._root, loc._root);
	if (!loc._index.empty())
		ruleblock._index = loc._index;
	if (!loc._allow_methods.empty())
		ruleblock._allow_methods = loc._allow_methods;
	if (!loc._cgi_extension.empty())
		ruleblock._cgi_extension = loc._cgi_extension;
	if (!loc._cgi_path.empty())
		ruleblock._cgi_path = loc._cgi_path;
	ruleblock._autoindex = loc._autoindex; //default on extra
}


void  Request::checkRules(location &rules)
{   
	if (!rules.getReturn().empty())
	{
		this->_filePath = rules.getReturn();
		_statusCode = 301;
		return;
	}
	else if (!rules.getAllowMethods().empty())
	{
		std::vector<Http_method> allow_methods = rules.getAllowMethods();

		for (auto i : allow_methods)
		{
			if (i == this->_method_type)
				return;
		}
		throw (ClientErrorExcept(405, "Method not allowed"));
	}
	else if (!rules.getRoot().empty())
	{
		_filePath.find(_config->_rootDir);
		_filePath.replace(0, _config->_rootDir.size(), rules.getRoot());
	}
	else if (!rules.getAlias().empty())
		this->_filePath = rules.getAlias();
	else if (!rules.getIndex().empty())
	{
		std::filesystem::path p = _filePath;
		if (std::filesystem::is_directory(p))
		{
			std::vector<std::string> rules_index = rules.getIndex();
			std::string tmp = this->_filePath;

			for (auto i : rules_index)
			{
				tmp += i;
				if (fileExists(_filePath))
				{
					_filePath = tmp;
					return;
				}
				tmp = _filePath;
				if (rules.getAutoindex())
					this->_dirListing = true;
			}
		}
	}
	else if (!rules.getCgiExtension().empty())
	{
		std::vector<std::string> cgi_extension = rules.getCgiExtension();
		size_t pos = _filePath.rfind('.');
		std::string extension_to_compare = _filePath.substr(pos + 1);
		std::cout << extension_to_compare << std::endl;

		for (auto i : cgi_extension)
		{
			if (i == extension_to_compare)
				return;
		}
		throw (ClientErrorExcept(500, "Internal Server Error"));
	}
}


