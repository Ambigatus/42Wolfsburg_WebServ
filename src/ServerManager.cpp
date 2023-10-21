/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboichuk <hboichuk@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 16:43:35 by hboichuk          #+#    #+#             */
/*   Updated: 2023/10/21 17:56:28 by hboichuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ServerManager.hpp"

ServerManager::ServerManager(){}
~ServerManager::ServerManager(){}

//start all servers on ports, what specified in the config file
void    ServerManager::setupServers(std::vector<ServerConfig> servers){}



//start of all system
void	ServerManager::startServers()
{
	fd_set read_fds; // sockets that the server is interested in for reading
	fd_set write_fds; // sockets that the server is interested in for writing
	int num_ready_fds; // the number of file descriptors that are ready for reading or writing

	max_fd = 0;
	initializeFdsSets();
	//doesn't finished
}

/*init for all sets of fds for listening requests*/
void	ServerManager::initializeFdsSets()
{
	/*fd-zero - initializes the file descriptor set to contain no file descriptors*/
	FD_ZERO(&_read_fds);
    FD_ZERO(&_write_fds);
	std::vector<ServerConfig>::iterator it = _servers_config.begin();

	while(it != _servers_config.end())
	{
		
		++it;
	}
	//doesn't finished
}