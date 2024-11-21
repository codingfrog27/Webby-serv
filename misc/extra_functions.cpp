/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   extra_functions.cpp								:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/10/02 16:38:28 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/02 16:38:37 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "socket.hpp"

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
	//Send the client file content
	send(client_socket, html_content.c_str(), html_content.size(), 0);
	std::cout << YELLOW << "--------- HTML message sent ----------" << RESET << std::endl;

	file.close();
}
