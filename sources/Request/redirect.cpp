#include "everything.hpp"
#include <filesystem>
#include "socket.hpp"

bool	Request::RouteRuleHandler()
{
	location	*locPtr;
	location	reqRules;
	size_t		matchCount = 0;
	std::vector<location> *locVec = &this->_config->_locations;
	if (locVec->empty())
		return false;
	locPtr = findLocationMatch(*locVec, matchCount);
	if (locPtr == nullptr)
		return false;
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
	return true;
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
			return (ret);
		}
		else if (root != nullptr)
		{
			return (root);
		}
	}
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
	ruleblock._autoindex = loc._autoindex;
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
		_filePath = rules.getReturn();
		_rootless = true;
		_statusCode = 302;
		_statusStr = "302 Found";
		std::cout << "redirect return triggered" << std::endl;
		return;
	}
	isMethodAllowed(_method_type , rules.getAllowMethods());
	if (!rules.getAlias().empty()) {
		_rootless = true;
		_filePath = rules.getAlias();
	}
	else if (!rules.getRoot().empty())
		_root =  rules.getRoot();
	checkIndex(rules.getIndex(), rules.getAutoindex());
	if (!rules.getCgiExtension().empty())
	{
		std::vector<std::string> cgi_extension = rules.getCgiExtension();
		size_t pos = _filePath.rfind('.');
		if (pos == std::string::npos || pos == _filePath.size() - 1)
			throw (ClientErrorExcept(403, "403 Forbidden"));
		std::string extension_to_compare = _filePath.substr(pos);
		for (auto i : cgi_extension)
		{
			if (i == extension_to_compare)
			{
				_cgiRequired = true;
				return;
			}	
		}
		throw (ClientErrorExcept(403, "403 Forbidden"));
	}
}


void	Request::checkIndex(std::vector<std::string> &indexPages, bool	autoindex)
{
	std::string indexPath, dirPath;
	
	if (this->_rootless == false)
		dirPath = this->_root;
	dirPath += this->_filePath;
	
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