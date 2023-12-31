/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddzuba <ddzuba@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 15:28:43 by hboichuk          #+#    #+#             */
/*   Updated: 2023/12/23 17:11:47 by ddzuba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserv.hpp" // libraries
#include "../include/ServerManager.hpp" //main class for creating server

/*handle the SIGPIPE signal without any specific action*/
//void sigpipeHandle(int /* sig */) {
//     ERROR << "SIGPIPE received!" << std::endl;
//}
void sigpipeHandle(int sig) { if(sig) {}}

int	main(int argc, char **argv)
{
	if (argc == 1 || argc == 2)
	{
		try
		{
			STR			serv_configuration;
			ConfigurationParser	config_parser;
			ServerManager		manager;
			signal(SIGPIPE, sigpipeHandle);

            serv_configuration = (argc == 1 ? "conf/default.conf" : argv[1]);

			config_parser.parseConfig(serv_configuration);
//            config_parser.printConfig();
//            printf("I am here\n");
			manager.setupServers(config_parser.getServers());
			manager.startServers();

		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			return (1);
		}
	}
	else
	{
		Logger::messageLog(B_RED, CONSOLE_OUTPUT, "Error: wrong arguments.");
		return (1);
	}
	return (0);
}
