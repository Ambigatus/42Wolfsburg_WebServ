/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboichuk <hboichuk@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 15:28:43 by hboichuk          #+#    #+#             */
/*   Updated: 2023/11/05 17:12:00 by hboichuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserv.hpp" // libraries
#include "../include/ServerManager.hpp" //main class for creating server

int	main(int argc, char **argv)
{
	if (argc == 1 || argc == 2)
	{
		try
		{
			//place for config parser?
			// ServerManager main; // init server

			//place for setupServer
			//place for startServer
			
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			return (1);
		}
	}
	else
	{
		std::cerr << "Wrong arguments" << std::endl; //better error need
		return (1);
	}
}