#include "everything.hpp"
#include <filesystem>

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

// void    Request::checkForRedirect(std::string _filePath)
// {
//     std::cout << "This is the _filePath: " << _filePath << std::endl;

//     // if (_config != nullptr) 
//     //     // for (const auto& pair : _config->_locations)
//         //     std::cout << pair.first << '\t' << pair.second << std::endl;
//         for (const auto& pair : _config->_locations)
//             std::cout << pair << std::endl;
//     else if (_config == nullptr)
//         std::cerr << "Config is nullptr" << std::endl;
// }