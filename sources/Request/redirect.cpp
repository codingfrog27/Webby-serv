#include "everything.hpp"
#include <filesystem>
#include "socket.hpp"

void	Request::RouteRuleHandler()
{
	location	*locPtr;
	location	reqRules;
	size_t		matchCount = 0;
	// std::cout << YELLOW "path before routehnandler == " RESET << _filePath << std::endl;
	std::vector<location> *locVec = &this->_config->_locations;
	if (locVec->empty())
		return;
	locPtr = findLocationMatch(*locVec, matchCount);
	if (locPtr == nullptr)
		return;
	reqRules = *locPtr;
	if (!locPtr->_nestedLocations.empty()) 
	{
		for (; !locVec->empty(); locVec = &locPtr->_nestedLocations)
		{
			locPtr = findLocationMatch(*locVec, matchCount);
			if (locPtr == nullptr)
				break;
			setLocRules(reqRules, *locPtr);
		}
	}
	checkRules(reqRules);
}
 
location	*Request::findLocationMatch(std::vector<location> &locs, size_t &matchCount)
{
	size_t		newSize;
	location	*ret = nullptr, *root = nullptr;
	bool		matchFound = false;
	for (std::vector<location>::iterator it = locs.begin(); it != locs.end(); ++it)
	{
		std::string	&locpath = it->getName();
		if (locpath == "/")
			root = &(*it);
		newSize = countPathMatch(_filePath, locpath);
		if (newSize > matchCount)
		{
			// std::cout << MAGENTA "MATCH FOUND" RESET << std::endl;
			matchCount = newSize;
			matchFound = true;
			ret = &(*it);
			if (newSize == std::string::npos)
				break;
		}
	}
	if (matchFound)
	{
		if (matchCount > 1)
		{
			// std::cout << "REAL MATHC FOUND BBY match == " << ret->getName() << " reqpath == " << this->_filePath << std::endl;
			return (ret);
		}
		else if (root != nullptr)
		{
			// std::cout << "root match uwu" << std::endl;
			return (root);
		}
	}
	// std::cout << "NO MATHC FOUND wompwomp" << std::endl;
	return (nullptr);
}

size_t	Request::countPathMatch(std::string &reqpath, std::string &locpath)
{
	size_t	size = 0, matchCount = 0;
	for (;size < reqpath.size() && size < locpath.size()\
		&& reqpath[size] == locpath[size]; size++)
	{
		if (reqpath[size] == '/')
			matchCount++;
	}
	if (size == locpath.size() && size == reqpath.size())
		return (std::string::npos);
	return (matchCount);
}

void assignStrIfNonEmpty(std::string &dest, std::string &rhs)
{
	if (!rhs.empty())
		dest = rhs;
}

void	Request::setLocRules(location &ruleblock, location &loc)
{
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
	throw (ClientErrorExcept(405, "405 Method not allowed"));
}

void  Request::checkRules(location &rules)
{   
	if (!rules.getReturn().empty())
	{
		this->_filePath = rules.getReturn();
		_statusCode = 301;
		_statusStr = "301 Moved Permanently";
		return;
	}
	// isMethodAllowed(_method_type, rules.getAllowMethods());
	// if (!rules.getRoot().empty())
	// {
	// 	_filePath.find(_config->_rootDir);
	// 	_filePath.replace(0, _config->_rootDir.size(), rules.getRoot()); //???
	// }
	// else if (!rules.getAlias().empty()) //wrong, check merlin ss
	// 	this->_filePath = rules.getAlias();
	checkIndex(rules.getIndex(), rules.getAutoindex());
	// if (!rules.getCgiExtension().empty())
	// {
	// 	std::vector<std::string> cgi_extension = rules.getCgiExtension();
	// 	size_t pos = _filePath.rfind('.');
	// 	std::string extension_to_compare = _filePath.substr(pos + 1);
	// 	std::cout << extension_to_compare << std::endl;

	// 	for (auto i : cgi_extension)
	// 	{
	// 		if (i == extension_to_compare)
	// 			return;
	// 	}
	// 	throw (ClientErrorExcept(500, "Internal Server Error"));
	// }
}


void	Request::checkIndex(std::vector<std::string> &indexPages, bool	autoindex)
{
	std::string indexPath, dirPath = this->_root + this->_filePath;
	
	if (!std::filesystem::is_directory(dirPath))
		return;
	if (!indexPages.empty())
	{
		
		for (auto i : indexPages)
		{
			indexPath = dirPath + i;
			if (fileExists(indexPath))
			{
				_filePath += i;
				return;
			}
			indexPath = _filePath;
		}
	}
	if (autoindex)
	{
		this->_dirListing = true;
		return;
	}
	throw ClientErrorExcept(403, "403 Forbidden");
}