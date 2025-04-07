/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   uri_parsing.cpp									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/09/23 15:54:31 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/09/27 18:25:05 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */


#include <string>
#include <stdexcept>

std::string urlToFilePath(const std::string url)
 {
	// Ensure the URL starts with a '/'
	if (url.empty() || url[0] != '/') {
		throw std::invalid_argument("Invalid URL");
	}
	std::string sanitized_url;

	// Sanitize the URL to prevent directory traversal attacks
	for (size_t i = 0; i < url.size(); ++i) {
		if (url[i] == '.' && i + 1 < url.size() && url[i + 1] == '.') {
			//go into path change
			i++;
		}
	}

	return sanitized_url;
}
