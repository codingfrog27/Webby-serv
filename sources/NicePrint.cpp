/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nicePrint.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/08 20:09:30 by mde-cloe      #+#    #+#                 */
/*   Updated: 2024/08/08 20:09:30 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "NicePrint.hpp"

// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

// NicePrint::NicePrint(void)
// {
// 	// std::cout << GREEN << "NicePrint: Default constructor called" << RESET << std::endl;
// }

NicePrint::NicePrint(const NicePrint &rhs)
{
	std::cout << GREEN << "NicePrint: Copy constructor called" << RESET << std::endl;

	*this = rhs;
}

NicePrint &
NicePrint::operator=(const NicePrint &rhs)
{
	std::cout << GREEN << "NicePrint: Assignment operator called" << RESET << std::endl;

	if (this != &rhs)
	{
		// Perform deep copy
	}

	return (*this);
}

// NicePrint::~NicePrint(void)
// {
// 	// std::cout << RED << "NicePrint: Destructor called" << RESET << std::endl;
// }

// ************************************************************************** //
//                                Public methods                              //
// ************************************************************************** //




void	NicePrint::high_five_loop()
{
	std::string me = "<(▼▽▼)っ";
	std::string you = "୧(^ ᵕ ^   )っ";
	std::string epic = "E P I C";
	int distance = 20;
	while (distance > 0)
	{
		system("clear");
		std::cout << me;
		for (int i = 0; i < distance; ++i) std::cout << " ";
		std::cout << you << std::endl;
		if (distance % 2)
			me.insert(0, 1, ' ');
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		distance--;
	}
}

void	NicePrint::stagger_print(std::string msg, size_t ms)
{
	for (size_t i = 0; msg[i]; i++)
	{
		std::cout << msg[i] << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
}

void	NicePrint::count_down(int counter)
{
	while (counter > 0)
	{
		system("clear");
		std::cout << YEL << counter << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(450));
		counter--;
	}

}

void NicePrint::greeter()
{
	this->stagger_print(YEL LINE_S "---\n|starting program|\n---"\
						 LINE_S RESET "\n", 10);
	promptEnter();
}


void	NicePrint::printLine() const
{
	std::cout << YEL LINE RESET << std::endl;
}
void	NicePrint::promptEnter()
{	
	std::cout << "\n" YEL LINE << std::endl;
	std::cout << "|\t   Press Enter to continue...\t      |\n";
	std::cout << YEL LINE RESET << std::endl;
	std::cin.get();
}