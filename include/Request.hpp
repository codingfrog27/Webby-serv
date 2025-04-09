/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::		*/
/*   Request.hpp									:+:	  :+:	:+:			*/
/*													+:+ +:+		 +:+		 */
/*	By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+		+#+			*/
/*												+#+#+#+#+#+	+#+				*/
/*	Created: 2024/09/06 17:18:08 by mde-cloe		  #+#	#+#				 */
/*	Updated: 2024/10/03 18:18:36 by mde-cloe		 ###	########.fr		*/
/*																			*/
/* ************************************************************************** */


#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <unordered_map>
#include <sys/socket.h>
#include "location.hpp"


#include "libft.h"
#include "ClientErrorExcept.hpp"
#include "ConnectionClosedExcep.hpp"
#include "Config.hpp"
#include "timeout.hpp"

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"
#define BUFFER_SIZE 1024
#define PLACEHOLDER_MAX_SIZE 1000000

typedef std::vector<unsigned char>::iterator t_ucit;
enum class connectStatus;

enum reading_status
{
	NOT_STARTED,
	READING_HEADERS,
	READING_BODY,
	READING_BODY_CHUNKED,
	FINISHED
};

enum class Http_method
{
	INVALID = -1,
	GET,
	POST,
	DELETE,
	NOT_PARSED_YET,
};

class Request
{
	private:
		Config					*_config;
		std::string				_reqBody;
		std::string				_unsortedHeaders;
		reading_status			reading_mode;
		std::string				_boundary;
		size_t					body_bytes_read;
		bool					_rnrnFound;
		bool					_dataIsChunked;
		bool					_headerAreParsed;
		bool					_hasBody;
		size_t					_contentLen;
		size_t					_max_body_size;
		t_secs					_timeoutTime;
		t_time					_startTime;
		std::string				_root;



		int						readSocket(int size);
		void					parse_headers(std::string str);
		Http_method				which_method_type(std::string str);
		bool					headerEndFound();
		size_t					parse_req_line(std::string req_line);
		bool					bodyIsRead();
		bool					isCGIrequired();
		bool					dechunkBody();
		std::string				http_version(const std::string &version);
		void					checkHeaders();
		void					checkBodyHeaders();
		void					parseBody();
		void					parseFormData(std::string &content_type);
		void					parseUrlEncoded();
		int						convertChunkSize(const std::string &hexStr, size_t &bytesRead);
		void					resolveFilePath();
		void					RouteRuleHandler();
		location				*findLocationMatch(std::vector<location> &locs, size_t &matchCount);
		void					setLocRules(location &loc, location &ruleblock);
		size_t					countPathMatch(std::string &reqpath, std::string &locpath);
		void  					checkRules(location &rules);

	public:
		int							_clientFD;
		std::vector<unsigned char>	_rawRequestData;
		Http_method					_method_type;
		std::unordered_map\
		<std::string, std::string>	_headers;
		std::unordered_map\
		<std::string, std::string>	_wwwFormEncodedPairs;
		std::string					_http_version;
		std::string					request_line;
		std::string 				_URI;
		std::string					_filePath;
		bool						_keepOpen;
		bool						_doneReading;
		std::string					_statusStr;
		int							_statusCode;
		bool						_dirListing;

						Request(void) = delete;
						Request(Config *config, int _clientFD);
						Request(const Request &rhs);
		Request 		&operator=(const Request &rhs);
						~Request(void);

		const std::string	&getBody();

		connectStatus	readRequest();
		std::string		getHeaderValue(std::string key);
		bool			headerExists(std::string key);
		std::string		getStatusCode();
		void			printHeaders();

		Config*			getConfig();
		void			checkIndex(std::vector<std::string> &indexPages, bool	autoindex);
};

std::string trim(const std::string& str);
