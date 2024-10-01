#include "Response.hpp"
#include "/home/mstegema/Documents/Webby-serv/include/HttpRequest.hpp"

int	main(void)
{
	int test_fd = open("/home/mstegema/Documents/Webby-serv/test.txt", 0);
	if (test_fd <= 0)
		std::cerr << "open failed" << std::endl;
	HttpRequest request(test_fd);
	responseHandler(&request);
	return 0;
}