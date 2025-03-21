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

std::ofstream file("/home/mde-cloe/PROJECTS/Webby-serv/sources/Request/testing");
// void create_testing_file(const std::string& content)
// {
// 	if (file.is_open())
// 	{
// 		file << content;
// 		file.close();
// 	}
// 	else
// 	{
// 		std::cerr << "Unable to open file for writing" << std::endl;
// 	}
// }

void	Request::RouteRuleHandler()
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
		if (newSize > matchCount)
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
	size_t	size = 0, matchCount = 0;
	for (;size < reqpath.size() && size < locpath.size()\
	 && reqpath[size] == locpath[size]; size++) {
		if (reqpath[size] == '/')
			matchCount++;
	 }
	if (size == locpath.size() && size == reqpath.size()){
			file << "returning npos for path " << reqpath << std::endl;
		return (std::string::npos);
	}
	file << "returning " << size << "for path " << reqpath << std::endl;	
	return (matchCount);
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

void isMethodAllowed(Http_method method, std::vector<Http_method> const &allow_methods)
{
	if (allow_methods.empty())
		return;
	for (auto i : allow_methods)
	{
		if (i == method)
			return;
	}
	throw (ClientErrorExcept(405, "Method not allowed"));
}

void  Request::checkRules(location &rules)
{   
	if (!rules.getReturn().empty())
	{
		this->_filePath = rules.getReturn();
		_statusCode = 301;
		return;
	}
	isMethodAllowed(_method_type, rules.getAllowMethods());
	if (!rules.getRoot().empty())
	{
		_filePath.find(_config->_rootDir);
		_filePath.replace(0, _config->_rootDir.size(), rules.getRoot());
	}
	else if (!rules.getAlias().empty()) //wrong, check merlin ss
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


