/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboichuk <hboichuk@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 16:43:35 by hboichuk          #+#    #+#             */
/*   Updated: 2023/10/14 17:01:38 by hboichuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ServerManager.hpp"

ServerManager::ServerManager(){}
~ServerManager::ServerManager(){}

//start all servers on ports, what specified in the config file

void    ServerManager::setupServers(std::vector<ServerConfig> servers){}




void	ServerManager::startServers()
{
	fd_set read_fds; // sockets that the server is interested in for reading
	fd_set write_fds; // sockets that the server is interested in for writing
	int num_ready_fds; // the number of file descriptors that are ready for reading or writing

	
}

