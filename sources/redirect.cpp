#include "everything.hpp"

#include "socket.hpp"

void sendDirectoryListing(int client_socket, const std::string& _filePath) {
    std::ostringstream html;
    std::ostringstream http_response;

    if (!std::filesystem::exists(_filePath)) 
    {
        http_response << "HTTP/1.1 404 Not Found\r\n";
        http_response << "Content-Type: text/html\r\n";
        http_response << "Content-Length: 25\r\n";
        http_response << "Connection: close\r\n";
        http_response << "\r\n";
        html << "<html><body><h1>Directory not found</h1></body></html>\n";
    }
    else if (!std::filesystem::is_directory(_filePath)) 
    {
        http_response << "HTTP/1.1 400 Bad Request\r\n";
        http_response << "Content-Type: text/html\r\n";
        http_response << "Content-Length: 21\r\n";
        http_response << "Connection: close\r\n";
        http_response << "\r\n";
        html << "<html><body><h1>Not a directory</h1></body></html>\n";
    } 
    else 
    {
        http_response << "HTTP/1.1 200 OK\r\n";
        http_response << "Content-Type: text/html\r\n";
        html << "<html><head><title>Directory Listing</title></head><body>\n";
        html << "<h1>Directory listing for " << _filePath << "</h1>\n";
        html << "<ul>\n";
        
        for (const auto& entry : std::filesystem::directory_iterator(_filePath)) {
            std::string entry_name = entry.path().filename().string();
            if (entry.is_directory()) {
                html << "<li><a href='" << entry_name << "/'>" << entry_name << "/</a></li>\n";
            } else {
                html << "<li><a href='" << entry_name << "'>" << entry_name << "</a></li>\n";
            }
        }
        
        html << "</ul>\n";
        html << "</body></html>\n";
    }
    std::string html_content = html.str();
    http_response << "Content-Length: " + std::to_string(html_content.size()) + "\r\n";
    http_response << "\r\n";

    send(client_socket, http_response.str().c_str(), http_response.str().size(), 0);
    send(client_socket, html_content.c_str(), html_content.size(), 0);
}

void printConfig(Config *configs)
{		
	std::vector<std::string> serverIndex = configs->getIndex();
       std::cout << "Server Index: ";
       for (const auto &idx : serverIndex) {
           std::cout << idx << ' ';
       }
	std::cout << std::endl;
	std::unordered_map<std::string, std::string> errorPage = configs->getErrorPage();
       std::cout << "Server ErrorPage: ";
       for (const auto &idx : errorPage) {
           std::cout << idx.first << " " << idx.second;
       }
	std::cout << std::endl;
	std::cout << LINE << std::endl;
       for (size_t j = 0; j < configs->_locations.size(); j++) {
           std::cout << "Location block:\n" << configs->_locations[j].toString();
           std::vector<Http_method> locationAllowMethods = configs->_locations[j].getAllowMethods();
           std::cout << "Allow Methods: ";
           for (const auto &method : locationAllowMethods) {
               if (method == Http_method::GET)
                   std::cout << "GET" << ' ';
               else if (method == Http_method::POST)
                   std::cout << "POST" << ' ';
               else if (method == Http_method::DELETE)
                   std::cout << "DELETE" << ' ';
           }
		std::cout << std::endl; 
           std::vector<std::string> locationCgiExtensions = configs->_locations[j].getCgiExtension();
           std::cout << "CGI Extensions: ";
           for (const auto &extension : locationCgiExtensions) {
               std::cout << extension << ' ';
           }
		std::cout << std::endl;
           std::vector<std::string> locationCgiPaths = configs->_locations[j].getCgiPath();
           std::cout << "CGI Paths: ";
           for (const auto &path : locationCgiPaths) {
               std::cout << path << ' ';
           }
		std::cout << std::endl;
           std::vector<std::string> locationIndex = configs->_locations[j].getIndex();
           std::cout << "Index: ";
           for (const auto &idx : locationIndex) {
               std::cout << idx << ' ';
           }
		std::cout << std::endl;
		
		std::cout<< LINE << std::endl;
		// std::cout << "This is the nestedLocations size: " << configs->_locations[j]->_nestedLocations.size() << std::endl;
           for (size_t k = 0; k < configs->_locations[j]._nestedLocations.size(); k++) {
               std::cout << "NESTED location block: " 
                         << configs->_locations[j]._nestedLocations[k].toString() << std::endl;
		std::cout<< LINE << std::endl;
           }
       }
}

void assignStrIfNonEmpty(std::string &dest, std::string &rhs)
{
       if (!rhs.empty())
               dest = rhs;
}

void   Request::setLocRules(location &loc, location &ruleblock)
{
       // assignStrIfNonEmpty(ruleblock._allow_methods, loc._allow_methods); //change after merge
       // assignStrIfNonEmpty(ruleblock._index, loc._index);
       
        assignStrIfNonEmpty(ruleblock._alias, loc._alias);
        assignStrIfNonEmpty(ruleblock._return, loc._return);
        assignStrIfNonEmpty(ruleblock._root, loc._root);
        if (loc.getAutoindex())
            ruleblock.setAutoindex(true);
        if (!loc.getAllowMethods().empty())
            ruleblock.setAllowMethods(loc.getAllowMethods());
        if (!loc._index.empty())
                ruleblock._index = loc._index;
        if (!loc._cgi_extension.empty())
                ruleblock._cgi_extension = loc._cgi_extension;
        if (!loc._cgi_path.empty())
                ruleblock._cgi_path = loc._cgi_path;
}

size_t Request::countPathMatch(std::string &reqpath, const std::string &locpath)
{
       size_t  size = 0;
       // while (locpath.at(size) == reqpath.at(size))
       for (;size < reqpath.size() && size < locpath.size()\
        && reqpath[size] == locpath[size]; size++)
       if (size == reqpath.size())
               return (std::string::npos);
        return(size);
}

location       *Request::findLocationMatch(std::vector<location> &locs, size_t &matchCount)
{
   size_t  newSize;
   location *ret;
   std::string tmp = _filePath;
   std::string filepath_to_check;

    auto first_ws = tmp.find_first_of('/');
    if (first_ws == std::string::npos)
        return (nullptr);
    auto word_begin = tmp.find_first_not_of('/', first_ws);
    if (first_ws == std::string::npos)
        return (nullptr);
    auto word_end = tmp.find_first_of('/', word_begin);
    filepath_to_check = tmp.substr(word_begin - 1, word_end - word_begin + 2);

    for (std::vector<location>::iterator it = locs.begin(); it != locs.end(); ++it)
   {
           newSize = countPathMatch(filepath_to_check, it->getName());
           if (newSize > matchCount)
           {
                matchCount = newSize;
                ret = &(*it);
                if (newSize == std::string::npos)
                    break;
           }
   }
   if (newSize == 0)
           return (nullptr);
   return (ret);
}

void   Request::locationHandler()
{
       location        *reqRules;
       location        *nestRules;
    //    size_t          matchCount = 0;
       std::vector<location> &locVec = this->_config->_locations;
       std::cout << "current req _filePath == (b4 loc-check) " << _filePath << std::endl \
       << "FD == " << _clientFD << std::endl;
       if (locVec.empty())
               return;
       reqRules = findLocationMatch(locVec, matchCount);
       if (reqRules == nullptr)
            return;
        std::cout << std::filesystem::is_directory(_filePath) << std::endl;
        if (!reqRules->getAutoindex() && std::filesystem::is_directory(_filePath))
        {
            std::cout << "Directory requested: Generating directory listing for " << _filePath << std::endl;
            sendDirectoryListing(_clientFD, _filePath);
        }
       locVec = reqRules->_nestedLocations;
       while (!locVec.empty())
       {
            nestRules = findLocationMatch(locVec, matchCount);
            if (nestRules == nullptr)
                break;
            checkRules(*nestRules);
            setLocRules(*reqRules, *nestRules);
            locVec = nestRules->_nestedLocations;
       }
    checkRules(*reqRules);
    // setLocRules(*reqRules, *nestRules);
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

