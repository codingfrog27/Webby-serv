#include "everything.hpp"
#include <filesystem>
#include "socket.hpp"

std::string generate_directory_listing(const std::string& _filePath) //should become response func 
{
    std::ostringstream html;

    if (!std::filesystem::exists(_filePath)) {
        // std::cout << "Do you go here?" << std::endl;
        std::cout << _filePath << std::endl;
        return "<html><body><h1>Directory not found</h1></body></html>\n";
    }
    
    if (!std::filesystem::is_directory(_filePath)) {
        // std::cout << "Do you go HERE?" << std::endl;
       
        std::string html_content = "<html><body><h1>Not a directory</h1></body></html>\n";

        std::string http_response = "HTTP/1.1 200 OK\r\n";
	    http_response += "Content-Type: text/html\r\n";
	    http_response += "Content-Length: " + std::to_string(html_content.size()) + "\r\n";
	    http_response += "Connection: close\r\n";
	    http_response += "\r\n";
    }

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

void sendHTMLPage(int client_socket, const std::string& file_path) 
{
	std::string html_content = file_path;

	//HTTP Response Headers
	std::string http_response = "HTTP/1.1 200 OK\r\n";
	http_response += "Content-Type: text/html\r\n";
	http_response += "Content-Length: " + std::to_string(html_content.size()) + "\r\n";
	http_response += "Connection: close\r\n";
	http_response += "\r\n";

	//Send the HTTP header
	send(client_socket, http_response.c_str(), http_response.size(), 0);
	//Send the client file content      /// add html_content to http_response!!!!!!!
	send(client_socket, html_content.c_str(), html_content.size(), 0);
	std::cout << YELLOW << "--------- HTML message sent ----------" << RESET << std::endl;
    std::cout << html_content << std::endl;

	// file.close();
}

void	Request::send_directory_listing_page(std::string html_page)
{
	// std::fstream	File;

	// File.open("directory_listing.html", std::ios::out);
	// File << html_page;
	// File.close();
    std::string html_to_send = generate_directory_listing(html_page);

    // send(_clientFD, html_to_send.c_str(), html_to_send.size(), 0);
    sendHTMLPage(_clientFD, html_to_send);
}


void printConfig(Config *configs)
{
    // for (size_t i = 0; i < configs.size(); i++) {
    //     std::cout << "Server block:\n" << configs[i].toString();
		
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
    // }
}

// void   Request::checkLocationMatch(std::vector<location> &locs, location &ruleblock)
// { 
//     for (size_t i = 0; i < locs.size(); i++)
//    {
//         if (_filePath.find(locs[i].getRoot()))
//             setLocRules(locs[i], ruleblock);
//     }
// }

// void   Request::setLocRules(location &loc, location &ruleblock)
// {
//     if ()
//     ruleblock.
//     checkLocationMatch(loc._nestedLocations, ruleblock);

// }

void  Request::checkRules(location &rules)
{   //loop through allowed methods to check if is allowed
    // std::cout << "This is the rulemap: " << std::endl;
    
    std::cout << this->_filePath << std::endl;

    // if (!rules.getAlias().empty())
    //     this->_filePath = rules.getAlias();
    // if (!rules.getReturn().empty())
    // {
    //     this->_filePath = rules.getReturn();
    //     _statusCode = 301;
    // }
    if (!rules.getIndex().empty())
    {
        if (_filePath.find(".html"))
        {
            size_t path_to_check = _filePath.rfind('/');
            std::string string_to_check = _filePath.substr(path_to_check + 1);
            std::vector<std::string> rules_index = rules.getIndex();

            // std::cout << "This is string to check: " << string_to_check << std::endl;
            for (auto i : rules_index)
            {
                // std::cout << i << std::endl;
                if (i != string_to_check)
                {
                    if (rules.getAutoindex())
                    {
                        // std::cout << _filePath << std::endl;
                        send_directory_listing_page(_filePath);
                    }
                }
    
            }
        }
            
    }
    // if (!rules.getAlias().empty())
    // {
    //     this->_config->_locations.
    // }
    // for (size_t i = 0; i < count; i++)
    // {
    //         /* code */
    // }
    //if redirect change path to redirect path
    //if autoindex && filepath == folder change path to index
    //alias?? maybe cgi??
}

void    Request::checkForRedirect(std::string _filePath)
{
    (void) _filePath;
    // std::cout << "This is the _filePath: " << _filePath << std::endl;

    checkRules(this->_config->_locations[0]);
    // if(this->_config->_locations[0]._nestedLocations[0]._return != "")
    //     std::cout << "this is the Redirect" << this->_config->_locations[0]._nestedLocations[0]._return << std::endl;
  
    // Config *config = getConfig();

	// std::cout << "Hello!" << std::endl;
	// printConfig(config);

    

    // if (_config != nullptr)
    // {
    //     // sendHTMLPage(_clientFD, _filePath);
    // }
    // else if (_config == nullptr)
    //     std::cerr << "Config is nullptr" << std::endl;
}
