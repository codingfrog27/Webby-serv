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

// 100 continue
// 102 Processing



// 200 OK
// 201 created

//if going out of root dir == 403
// 404 not found
// 405 when trying to delete
// 408 request timeout

//415 unsupported media type
//429 too many requests


//maybe need these?

// 204 no content == post/delete succesfull but dont need to load a new page
//301/302 moved url

// 416 Range not satifiable
// 417 expectation failed

//424 failed dependency


//431 request header fields too large