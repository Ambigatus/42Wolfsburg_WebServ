/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboichuk <hboichuk@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 16:43:35 by hboichuk          #+#    #+#             */
/*   Updated: 2023/10/22 19:39:56 by hboichuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ServerManager.hpp"

ServerManager::ServerManager(){}
~ServerManager::ServerManager(){}

//start all servers on ports, what specified in the config file
void    ServerManager::setupServers(std::vector<ServerConfig> _servers_config){


/*part for listen fd, we can delete if we use var from ServerConfig Class!*/
	// After creating the listening socket, add it to _listen_fds along with its associated server ID.
        _listen_fds[serverConfig.getFd()] = serverConfig.getId();

        // Also, add the server configuration to the _servers vector.
        _servers.push_back(serverConfig);
/*end of this part*/
}



//start of all system
void	ServerManager::startServers()
{
	fd_set read_fds; // sockets that the server is interested in for reading
	fd_set write_fds; // sockets that the server is interested in for writing
	int num_ready_fds; // the number of file descriptors that are ready for reading or writing

	_max_fd = 0;
	initializeFdsSets();
	//doesn't finished
}

/*init for all sets of fds for listening requests*/
void	ServerManager::initializeFdsSets()
{
	/*fd-zero - initializes the file descriptor set to contain no file descriptors*/
	FD_ZERO(&_read_fds_set);
    FD_ZERO(&_write_fds_set);
	std::vector<ServerConfig>::iterator it = _servers_config.begin();

	while(it != _servers_config.end())
	{
		if (listen(getListenFd(it->getId()), 512) == -1)
		{
			//we need logger!
			std::cerr << "Error: " << "Problems with setting up the listening socket" << std::endl;
    		exit(EXIT_FAILURE);
		}
		/*O_NONBLOCK-flag make the file descriptor non-blocking. It means that operations
		 on this file descriptor (like reading or writing) won't block the calling process.*/
		if (fcntl(getListenFd(it->getId()), F_SETFL, O_NONBLOCK) < 0)
        {
            std::cerr << "Error: " << "Can't make current fd unblocked" << std::endl;
            exit(EXIT_FAILURE);
        }
		addToSet(getListenFd(it->getId()), _read_fds_set);
		/*inserting an entry into the _servers_map with new key-value pair*/
		_servers_map.insert(std::make_pair(getListenFd(it->getId()), *it));
		++it;
	}
	/*max_fd -> last created server*/
	_max_fd = _servers.back().getListenFd(it->getId());
}

/*part for listen fd, we can delete if we use var from ServerConfig Class!*/
int ServerManager::getListenFd(int serverId) {
    std::map<int, int>::iterator it = _listen_fds.find(serverId);
    if (it != _listen_fds.end()) {
        return it->first;  // This is the listening file descriptor.
    }
    return -1;  // Handle the case where the server ID is not found.
}
/*end of this part*/

/* utility function used to add file descriptors to a set and maintain information 
about the largest file descriptor in the set by updating _max_fd. */
void	ServerManager::addToSet(const int i, fd_set &set)
{
	/*This line adds the file descriptor i to the file descriptor set */
    FD_SET(i, &set);
    if (i > _max_fd)
        _max_fd = i;
}