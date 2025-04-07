#include <string>



void	printStuff()
{
	std::string		content_type = getHeaderValue("Content-Type");
		std::cout << LILAC "path " << _filePath << std::endl;
		std::cout << "content type: " << content_type << std::endl;
		std::cout << "content length: " << _contentLen << std::endl;
		std::cout << "body: " << _reqBody << std::endl;
		std::cout << "raw data: " RESET << std::string(_rawRequestData.begin(), _rawRequestData.end()) << std::endl;

}


// void Server::PrintConnectionStatusses(size_t size)
// {
// 	for (size_t i = 0; i < size; i++)
// 	{
// 		// std::cout << "This is i: " << i << std::endl;
// 		// std::cout << "This is after close size: "<< size << std::endl;
// 		// Checking each possible enum value
// 		if (_Connections[i]._CStatus == connectStatus::SERV_SOCKET) {
// 			// Handle SERV_SOCKET status
// 			std::cout << "This is Server socket" << std::endl;
// 			// Code for handling SERV_SOCKET
// 		} else if (_Connections[i]._CStatus == connectStatus::IDLE) {
// 			// Handle IDLE status
// 			std::cout << "This is IDLE" << std::endl;
// 			// Code for handling IDLE
// 		} else if (_Connections[i]._CStatus == connectStatus::READING) {
// 			// Handle READING status
// 			std::cout << "This is 3" << std::endl;
// 			// Code for handling READING
// 		} else if (_Connections[i]._CStatus == connectStatus::REQ_ERR) {
// 			// Handle REQ_ERR status
// 			std::cout << "This is 4" << std::endl;
// 			// Code for handling REQ_ERR
// 		} else if (_Connections[i]._CStatus == connectStatus::CONNECT_CLOSED) {
// 			// Handle CONNECT_CLOSED status
// 			std::cout << "This is 5" << std::endl;
// 			// Code for handling CONNECT_CLOSED
// 		} else if (_Connections[i]._CStatus == connectStatus::DONE_READING) {
// 			// Handle DONE_READING status
// 			std::cout << "This is 6" << std::endl;
// 			// Code for handling DONE_READING
// 		} else if (_Connections[i]._CStatus == connectStatus::CGI_REQUIRED) {
// 			// Handle CGI_REQUIRED status
// 			std::cout << "This is 7" << std::endl;
// 			// Code for handling CGI_REQUIRED
// 		} else if (_Connections[i]._CStatus == connectStatus::RESPONDING) {
// 			// Handle RESPONDING status
// 			std::cout << "This is 8" << std::endl;
// 			// Code for handling RESPONDING
// 		} else if (_Connections[i]._CStatus == connectStatus::SERVER_ERR) {
// 			// Handle SERVER_ERR status
// 			std::cout << "This is 9" << std::endl;
// 			// Code for handling SERVER_ERR
// 		} else if (_Connections[i]._CStatus == connectStatus::FINISHED) {
// 			// Handle FINISHED status
// 			std::cout << "This is 10" << std::endl;
// 			// Code for handling FINISHED
// 		}
// 		}
// 	}
// }

// void Server::closeCGIConnects(){
// 	size_t	size;
// 	size = _CGIPollFDs.size();
// 	for (size_t i = 0; i < size; i++){
// 		CGI *cgi = _CGIMap[_CGIPollFDs[i].fd].get();
// 		if (cgi == nullptr){
// 			std::cout << "CGI is nullptr" << std::endl;
// 			continue;
// 		}
// 		Connection &connection = _Connections.at(cgi->getClientFD());
// 		if (_CGIPollFDs[i].revents & POLLHUP || (cgi->getCGIHandlerStatus() == CGIHandlerStatus::CHILD_IS_FINISHED && !_CGIPollFDs[i].revents & POLLIN)){
// 			if (_CGIPollFDs[i].revents & POLLHUP)
// 				close(_CGIPollFDs[i].fd );
// 			_CGIMap.erase(_CGIPollFDs[i].fd);
// 			_CGIPollFDs.erase(_CGIPollFDs.begin() + i);
// 			size--;
// 			i--;
// 			continue;
// 		}
// 	}
// }




//remove _CGIPollFDs[i] when finished in read or write function

// if (_pollFDs[i].fd != _Connections[i]._clientFD)
// {
// 	std::cout << "FD MISMATACH OH NO" << std::endl;
// 	NicePrint::promptEnter();
// }
// close(_Connections[i]._clientFD);
// _Connections.erase(_Connections.begin() + i);
// _pollFDs.erase(_pollFDs.begin() + i);


// std::cout << MAGENTA "CGI PollFD vector size in handleCGIPollEvents: " << _CGIPollFDs.size() << RESET << std::endl;
		// std::cout << "This is the fd left: " << _CGIPollFDs[i].fd << std::endl;

		// std::cout << "stdin: " << cgi->getFdIn() << std::endl;
		// std::cout << "stdout: " << cgi->getFdOut() << std::endl;
		// std::cout << "stderr: " << cgi->getFdError() << std::endl;



		//close connect stuff

		// if (_pollFDs.size() != _Connections.size())
		// std::cout << RED "VECTOR SIZE MISMAtCH BRO" RESET << std::endl;

		// 	std::cout << "CLOSED CONNECT with FD: " \
				<< fd << " With index: " << std::endl;




				// ************************************************************************** //
//								Public methods							  //
// ************************************************************************** //

	// signal(SIGPIPE, SIG_IGN);

	//refactor -> close connect loop 1 function, and for loops
		//if poll timeout-> get current time
		// std::cout << "This is start size: "<< size << std::endl;
		// PrintConnectionStatusses(size);


		//could mb be part of main loop after all?

		void writeClientFD(int clientFD, int i);

void writeClientFD(int clientFD, int i)
{
	std::ofstream outFile("clientFD_log.txt", std::ios::app);
		outFile << "Accepted new connection with clientFD: " << clientFD <<\
		" on index" << i << std::endl;
		// Example of using the address of clientFD for logging or other purposes
		// outFile << "Address of clientFD: " << &clientFD << std::endl;
		outFile.close();
}

writeClientFD(clientFD, i); //add me in acceptnew connects