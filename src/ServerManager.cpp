/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboichuk <hboichuk@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 16:43:35 by hboichuk          #+#    #+#             */
/*   Updated: 2023/11/01 17:18:32 by hboichuk         ###   ########.fr       */
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
	fd_set _read_fds; // sockets that the server is interested in for reading
	fd_set _write_fds; // sockets that the server is interested in for writing
	int num_ready_fds; // the number of file descriptors that are ready for reading or writing
	int	select_response;//select() answer
	int i;//for counting fds
	_max_fd = 0;
	
	initializeFdsSets();
	struct timeval timer;//how long select() will wait for ready fd
	while (true)
	{
		timer.tv_sec = 1;//select() will wait 1 second for any activity on fd
        timer.tv_usec = 0;
        read_fds = _read_fds_set;
        write_fds = _write_fds_set;
		/*select() is used for monitoring file descriptors (e.g., sockets) to
		 see if they are ready for reading, writing, or have encountered exceptions*/
		if ( (select_response = select(_max_fd + 1, &read_fds, &write_fds, NULL, &timer)) < 0 )
		{
			std::cerr << "Error: " << "Select doesn't work" << std::endl;
            exit(1);
			continue ;
		}
		i = 0;
		while (i <= _max_fd)
		{
			/* If this file descriptor corresponds to a server - we get new connection */
			if (FD_ISSET(i, &read_fds) && _servers_map.count(i))
                getNewConnection(_servers_map.find(i)->second);
			else if (FD_ISSET(i, &read_fds) && _clients_map.count(i))
				readRequest(i, _clients_map[i]);
			//doesn't finished
			i++;
		}
		
	}
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

/* utility function used to delete file descriptors from a set and maintain information 
about the largest file descriptor in the set by updating _max_fd. */
void	ServerManager::removeFromSet(const int i, fd_set &set)
{
    FD_CLR(i, &set);
    if (i == _max_fd)
        _max_fd--;
}

/*accepts a new client connection, logs the connection information, 
sets up the socket for non-blocking I/O, 
and keeps track of the connected clients in _clients_map.*/
void	ServerManager::getNewConnection(ServerConfig &server)
{
	/* most used address family, gives a socket an IPv4 socket address to
	allow it to communicate with other hosts over a TCP/IP network*/
	struct	sockaddr_in _client_ip_and_port;
	/* socklen_t - for storing sizes related to socket address structures */
	socklen_t	client_address_size = sizeof(client_address);
	/*for the newly accepted client connection*/
	int			client_socket;
	
	Client	new_client(server);
	
	/*INET_ADDRSTRLEN: constant typically from <netinet/in.h> for 
	IPv4 address(16 symbols)*/
	char    readable_ip[INET_ADDRSTRLEN];
	/*accept(int sockfd, struct sockaddr *_Nullable restrict addr,
    socklen_t *_Nullable restrict addrlen) - accept incoming connections from clients*/
	client_socket = accept(server.getListenFd(it->getId()), (struct sockaddr *)&_client_ip_and_port,\
				 (socklen_t*)&client_address_size);
	if (client_socket == -1)
	{
		std::cerr << "Error: " << "Accept error" << std::endl;
		return ;
	}
	std::cout << "New connection!" << std::endl;

	addToSet(client_socket, _read_fds_set);
	
	/*makes socket unblocked*/
	if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Error: " << "Fcntl error" << std::endl;
		removeFromSet(client_socket, _read_fds_set);
		close(client_socket);
		return ;
	}
	
	new_client.setSocket(client_socket);
	/*if our client exist we rewrite it in map*/
	if (_clients_map.count(client_socket) != 0)
		_clients_map.erase(client_socket);
	_clients_map.insert(std::make_pair(client_socket, new_client));
}

/*Reads data from the client and provides it to the parser.
When the parser finishes or detects an error in the request,
the socket is transferred from the _read_fds_set to the _write_fds_set.
The response will be sent during the next iteration of the select() function.*/
void	Client::readRequest(const int &i, Client &client)
{
	/*used to temporarily store data read from a client's socket*/
	char	buffer[40000]; //special define?
	/*keep track of the number of bytes that are read from 
	the client's socket during the read operation*/
	int     bytes_read = 0;
	
	bytes_read = read(i, buffer, 40000);
	if (bytes_read == 0)
	{
		std::cerr << "Error: " << "Closed connection" << std::endl;
		closeConnection(i);
		return ;
	}
	//doesn't finished
}

/*closes the connection with the file descriptor 'i' and removes 
the corresponding client object from the '_clients_map'*/
void	ServerManager::closeConnection(const int i)
{
	if (FD_ISSET(i, &_write_fds_set))
        removeFromSet(i, _write_fds_set);
    if (FD_ISSET(i, &_read_fds_set))
        removeFromSet(i, _read_fds_set);
    close(i);
    _clients_map.erase(i);
}