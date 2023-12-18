/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboichuk <hboichuk@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 16:43:35 by hboichuk          #+#    #+#             */
/*   Updated: 2023/12/18 19:50:38 by hboichuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ServerManager.hpp"

ServerManager::ServerManager(){}
~ServerManager::ServerManager(){}

//start all servers on ports, what specified in the config file
void    ServerManager::setupServers(std::vector<ServerConfig> servers_config){


/*part for listen fd, we can delete if we use var from ServerConfig Class!*/
	// After creating the listening socket, add it to _listen_fds along with its associated server ID.
        _listen_fds[serverConfig.getFd()] = serverConfig.getId();

        // Also, add the server configuration to the _servers vector.
        _servers.push_back(serverConfig);
/*end of this part*/

	Logger::messageLog(U_YELLOW, CONSOLE_OUTPUT, "Start Webserver...");
	_servers = servers_config;
	char	buffer[INET_ADDRSTRLEN];
	/*check if we have server duplicates or not*/
	bool	flag;
	std::vector<ServerConfig>::iterator it = _servers.begin();
	while (it != _servers.end())
	{
		flag = false;
		std::vector<ServerConfig>::iterator it2 = _servers.begin();
		
		while(it2 != it)
		{
			if (it2->getHost() == it->getHost() && it2->getPort() == it->getPort())
			{
				it->setFd(it2->getFd());
				flag = true;
			}
			if (!flag)
				it->serverSetup();
			Logger::messageLog(U_YELLOW, CONSOLE_OUTPUT, "Server created...");
			++it2;
		}
		++it;
	}
}



//start of all system
void	ServerManager::startServers()
{
	fd_set _read_fds; // sockets that the server is interested in for reading
	fd_set _write_fds; // sockets that the server is interested in for writing
	int num_ready_fds; // the number of file descriptors that are ready for reading or writing
	int	select_response;//select() answer
	int i;//for counting fds
	int	cgi_state;
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
			Logger::messageLog(B_RED, CONSOLE_OUTPUT,  "Select() error!", strerror(errno));
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
			else if (FD_ISSET(i, &write_fds) && _clients_map.count(i))
			{
				cgi_state = _clients_map[i].response.getCgiState(); // 0->NoCGI 1->CGI 
				if (cgi_state == 1 && FD_ISSET(_clients_map[i].response._cgi_conf.pipe_in[1],&write_fds))
					sendCgiBody(_clients_map[i], _clients_map[i].response._cgi_conf);
				else if (cgi_state == 1 && FD_ISSET(_clients_map[i].response._cgi_conf.pipe_out[0], &read_fds))
					readCgiResponse(_clients_map[i], _clients_map[i].response._cgi_conf);
				else if ((cgi_state == 0 || cgi_state == 2) && FD_ISSET(i, &write_fds))
					sendResponse(i , _clients_map[i]);
			}
			i++;
		}
	}
	checkTimeout();
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
    		Logger::messageLog(B_RED, CONSOLE_OUTPUT, "Problems with setting up the listening socket - %s", \
								strerror(errno));
			exit(EXIT_FAILURE);
		}
		/*O_NONBLOCK-flag make the file descriptor non-blocking. It means that operations
		 on this file descriptor (like reading or writing) won't block the calling process.*/
		if (fcntl(getListenFd(it->getId()), F_SETFL, O_NONBLOCK) < 0)
        {
			Logger::messageLog(B_RED, CONSOLE_OUTPUT, "Can't make current fd %s unblocked", strerror(errno));
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
		std::cerr << "Error: " <<  << std::endl;
		Logger::messageLog(B_RED, CONSOLE_OUTPUT, "Accept error %s", strerror(errno));
		return ;
	}
	std::cout <<  << std::endl;
	Logger::messageLog(B_YELLOW, CONSOLE_OUTPUT, "New connection!");

	addToSet(client_socket, _read_fds_set);
	
	/*makes socket unblocked*/
	if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		Logger::messageLog(B_RED, CONSOLE_OUTPUT, "Fcntl error with %s", strerror(errno));
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
		Logger::messageLog(B_PINK, CONSOLE_OUTPUT, "Closed connection %d", i);
		closeConnection(i);
		return ;
	}
	else if (bytes_read < 0)
	{
		Logger::messageLog(B_RED, CONSOLE_OUTPUT, "Read problems %d. Error: %s", i, strerror(errno));
		closeConnection(i);
		return ;
	}
	else if (bytes_read != 0)
	{
		/*updates the timestamp of the last received message from the client. 
		This is used to monitor client activity and connection timeouts*/
		client.updateTime();
		/*data read from the client (stored in 'buffer') is passed to a 'request' object */
		client.request.feed(buffer, bytes_read);
		/*clears the buffer*/
		memset(buffer, 0, sizeof(buffer)); 
	}
	
	/*true - parsing completed and we can work on the response*/
	if (client.request.parsingDone() || client.request.errorCode())
	{
		assignServer(client);
		std::cerr <<  << std::endl;
		Logger::messageLog(B_TURQUOISE, CONSOLE_OUTPUT, "Request recived!" );
		client.buildResponse();
		if (client.response.getCgiState())
		{
			isReqBodyEmpty(client);
			addToSet(client.response._cgi_obj.pipe_in[1], _write_fds_set);
			addToSet(client.response._cgi_obj.pipe_out[0], _read_fds_set);
		}
		removeFromSet(i, _read_fds_set);
		addToSet(i, _write_fds_set);
	}
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

/*assigning a specific server configuration to a client based on the client's request*/
void	ServerManager::assignServer(Client &client)
{
	std::vector<ServerConfig>::iterator it = _servers_config.begin();
	
	while(it != _servers_config.end())
	{
		/*comparing the properties of the client's connection (host and port) 
		and the requested server name with the properties defined in a server configuration*/
		if (client.server.getHost() == it->getHost() && \
			client.server.getPort() == it->getPort() && \
			client.request.getServerName() == it->getServerName())
			{
				client.setServer(*it);
				return ;
			}
		it++;
	}
}

/*checks if clients have been quiet for too long. If they have,
 it ends their connection to free up server resources.*/
void	ServerManager::checkTimeout()
{
	std::map<int, Client>::iterator it = _clients_map.begin();

	while (it != _clients_map.end())
	{
		/*after 60 seconds we will clean the data*/
		if (time(NULL) - it->second.getLastTime() > CONNECTION_TIMEOUT)
		{
			Logger::messageLog(B_YELLOW, CONSOLE_OUTPUT, "Timeout %d. Closing connection", it->first);
			closeConnection(it->first);
			return ;
		}
		++it;
	}
}

//check this func!
void	ServerManager::sendResponse(const int &i, Client &client)
{
	size_t	bytes_num;
	std::string	response = client.response.getRes();
	if (response.length() >= 40000)
		bytes_num = write(i, response.c_str(), 40000)
	else
		bytes_num = write(i, response.c_str(), response.length());

	if (bytes_num < 0)
	{
		Logger::messageLog(B_RED, CONSOLE_OUTPUT, "Wrong sendResponse %s", strerror(errno));
		closeConnection(i);
	}
	else if (bytes_num == 0 || bytes_num == response.length())
	{
		Logger::messageLog(B_TURQUOISE, CONSOLE_OUTPUT, "Response sent");
		if (client.request.keepAlive() == false || client.request.errorCode() || \
			client.response.getCgiState())
		{
			Logger::messageLog(B_YELLOW, CONSOLE_OUTPUT, "Connection closed %d", i);
			closeConnection(i);
		}
		else
		{
			removeFromSet(i, _write_fds_set);
			addToSet(i, _read_fds_set);
			client.clearClient();
		}
	}
	else
	{
		client.updateTime();
		client.response.cutRes(bytes_num);
	}
}

/*CGI part*/
/*sent Request body to CGI script*/
void	ServerManager::sendCgiBody(Client &client, CgiHandler &cgi)
{
	/*keep track of the number of bytes sent by the write system call*/
	size_t	bytes_num;
	std::string	&req_body = client.request.getBody();

	if (req_body.length() == 0)
		bytes_num = 0;
	else if (req_body.length() >= 40000)
		bytes_num = write(cgi.pipe_in[1], req_body.c_str(), 40000);
	else
		bytes_num = write(cgi.pipe_in[1], req_body.c_str(), req_body.length());

	if (bytes_num < 0 )
	{
		Logger::messageLog(B_RED, CONSOLE_OUTPUT, "Something wrong with sending %s", strerror(errno));
		removeFromSet(cgi.pipe_in[1], _write_fds_set);
		close(cgi.pipe_in[1]);
		close(cgi.pipe_out[1]);
		client.response.setErrorResponse(500);//check this line
		
	}
	else if (bytes_num == 0 || bytes_num == req_body.length())
	{
		removeFromSet(cgi.pipe_in[1], _write_fds_set);
		close(cgi.pipe_in[1]);
		close(cgi.pipe_out[1]);
	}
	else
	{
		client.updateTime();
		req_body = req_body.substr(bytes_num);
	}
}

//check this function for new data!
void	ServerManager::readCgiResponse(Client &client, CgiHandler &cgi)
{
	char	buffer[80000];
	size_t	bytes_num;
	int		status;
	
	bytes_num = read(cgi.pipe_out[0], buffer, 80000);
	if (bytes_num == 0)
	{
		removeFromSet(cgi.pipe_out[0], _read_fds_set);
		close(cgi.pipe_in[0]);
		close(cgi.pipe_out[0]);
		waitpid(cgi.getCgiPid(), &status, 0);
		/*child termination status*/
		if(WEXITSTATUS(status) != 0)
			client.response.setErrorResponse(502);//invalid response
		client.response.setCgiState(2);
		if (client.response._response_content.find("HTTP/1.1") == std::string::npos)
			client.response._response_content.insert(0, "HTTP/1.1 200 OK\r\n");
		return ;
	}
	else if (bytes_num < 0)
	{
		Logger::messageLog(B_RED, CONSOLE_OUTPUT, "Wrong reading from CGI script: %s", strerror(errno));
		removeFromSet(cgi.pipe_out[0], _read_fds_set);
		close(cgi.pipe_in[0]);
		close(cgi.pipe_out[0]);
		client.response.setCgiState(2);
		client.response.setErrorResponse(500);
		return ;
	}
	else
	{
		client.updateTime();
		client.response._response_content.append(buffer,bytes_num);
		memset(buffer, 0, sizeof(buffer));
	}
}

void    ServerManager::isReqBodyEmpty(Client &client)
{
	std::string			tmp;
	std::fstream		file;
	std::stringstream	string_stream;
	
	if (client.request.getBody().length() == 0)
	{
		file(client.response._cgi_obj.getCgiPath().c_str());
		string_stream << file.rdbuf();
		tmp = string_stream.str();
		client.request.setBody(tmp);
		
	}
}
