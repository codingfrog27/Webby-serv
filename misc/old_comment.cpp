// test

// size_t parse_hex_string(const std::string& hex_string) {
// 	size_t value = 0;
// 	bool valid = true;

// 	// Check for "0x" prefix
// 	if (hex_string.substr(0, 2) != "0x" && hex_string.substr(0, 2) != "0X") {
// 		valid = false;
// 	}

// 	for (size_t i = 2; i < hex_string.size() && valid; i++) {
// 		char c = hex_string[i];
// 		if (isdigit(c)) {
// 			value = (value << 4) + (c - '0');
// 		} else if (isxdigit(c)) {
// 			value = (value << 4) + (toupper(c) - 'A' + 10);
// 		} else {
// 			valid = false;
// 			break;
// 		}
// 	}

// 	if (!valid) {
// 		// Handle invalid input, e.g., throw an exception or return an error code
// 	}

// 	return value;
// }w



// void	Server::handleEvents()
// {
// 	auto it = _Requests.find(i);  // Use find() to check if the key exists

// 	if (it != _Requests.end()) {
//     if ((pfds[i].revents & POLLOUT) && it->second._doneReading)
//         responseHandler(&(it->second));  // Access the value via iterator
    
//     if (!it->second._keepOpen)
//         close_connect(it->second, i);
// }
// }

	// _Requests[i].readRequest();
		// if (num_events < 0) 
		// {
		// 	std::cerr << RED << "Poll failed with error: " << strerror(errno) << RESET << std::endl;
		// 	break;
		// }

// void	Server::main_server_loop()
// {
	
// 	std::unordered_map<int, Request>\
// 	::iterator	it;
// 	int			num_events;
	
// 	num_events = poll(_pollFDs.data(), _pollFDs.size(), -1); //handle poll error
// 	if (num_events == 0) 
// 		std::cout << MAGENTA << "Poll timed out, no events to handle." << RESET << std::endl;
// 	else
// 		std::cout << "YIPPIE" << std::endl;
// 	for (size_t i = 0; i < MAX_CLIENT; ++i)
// 	{
// 		if (_pollFDs[i].revents & POLLIN) //take other events
// 		{
// 			it = _Requests.find(i);
// 			if (it == _Requests.end())
// 			{
// 				auto result = _Requests.emplace(i, acceptNewConnects());
// 				// catch construct error?
// 				it = result.first;
// 			}
// 			it->second.readRequest();
// 		}
// 		if (_pollFDs[i].revents & POLLOUT)
// 		{
// 			it = _Requests.find(i);
// 			if (it != _Requests.end() && it->second._doneReading) //can i put this if into the above condition?
// 			{
// 				responseHandler(&it->second);
// 				if (!it->second._keepOpen)
// 					close_connect(it->second, i);
// 			}
// 		}
// 		// else
// 			// refresh req object only
// 	}
// }













// Server::Server(const Server &rhs) :  _max_clients(rhs._max_clients)
// {
// 	std::cout << GREEN << "Server: Copy constructor called" << RESET << std::endl;

// 	*this = rhs;
// }

// Server &
// Server::operator=(const Server &rhs)
// {
// 	std::cout << GREEN << "Server: Assignment operator called" << RESET << std::endl;

// 	if (this != &rhs)
// 	{
// 		// Perform deep copy
// 	}

// 	return (*this);
// }
