/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:47:46 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/10 17:25:10 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "http_request.hpp"

int main()
{
	int test_fd = open("test.txt", 0);
	if (test_fd <= 0)
		std::cerr << "open failed" << std::endl;
	Http_request request(test_fd);
	return 0;
}
