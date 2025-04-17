#include "Response.hpp"
#include "Request.hpp"
#include <filesystem>

bool	fileExists(std::string path){
	std::ifstream file(path);
	if (file.is_open()){
		file.close();
		return true;
	}
	return false;
}

std::string Response::generateDirectoryListing(const std::string& _filePath)
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
				continue;
			html << "<li><a href='" << entry_name << "'>" << entry_name << "</a></li>\n";
		}
	}

	html << "</ul>\n";
	html << "</body></html>\n";

	return html.str();
}
