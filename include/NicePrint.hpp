/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nicePrint.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/08 20:11:59 by mde-cloe      #+#    #+#                 */
/*   Updated: 2024/08/08 20:11:59 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICEPRINT_HPP

#define NICEPRINT_HPP

#include <iostream>
#include <thread>
#include <chrono>

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define BLK "\e[0;30m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

#define LINE "-----------------------------------------------"
#define LINE_S "--------------"

class NicePrint {
public:
	// Constructors
	NicePrint() = default;
	NicePrint(const NicePrint &other);
	NicePrint& operator=(const NicePrint &other);
	~NicePrint()  = default;


	// Other member functions
	void	count_down(int counter);
	void	stagger_print(std::string msg, size_t ms);
	void	high_five_loop();
	void	greeter();
	static void	promptEnter();
	void	printLine() const;
	// void	newline();
	const std::string div = "--------------------------------------------";
	const std::string div_s = "--------------";

private:
	// Member variables
};
//const (and constexpr) can be defined directly in the header in c++11 and later :)

#endif // NICEPRINT_HPP
