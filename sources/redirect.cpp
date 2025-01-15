#include "everything.hpp"
#include <filesystem>
#include "socket.hpp"

std::string generate_directory_listing(const std::string& _filePath) 
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
		std::cerr << RED << "Error opening file: " << file_path << RESET << std::endl;
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
	//Send the client file content      /// add html_content to http_response!!!!!!!
	send(client_socket, html_content.c_str(), html_content.size(), 0);
	std::cout << YELLOW << "--------- HTML message sent ----------" << RESET << std::endl;

	file.close();
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
            std::cout << "Allow Methods:";
            for (const auto &method : locationAllowMethods) {
                if (method == Http_method::GET)
                    std::cout << "ayy its get" << ' ';
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

    

    if (_config != nullptr)
    {
        // sendHTMLPage(_clientFD, _filePath);
    }
    else if (_config == nullptr)
        std::cerr << "Config is nullptr" << std::endl;
}

void    Request::checkRules(location &rules)
{	//loop through allowed methods to check if is allowed

    (void)rules;
      Config *config = getConfig();
     	// std::cout << "Hello!" << std::endl;
	 printConfig(config);
    // rules._allow_methods
	//if redirect change path to redirect path
	//if autoindex && filepath == folder change path to index
	//alias?? maybe cgi??
}