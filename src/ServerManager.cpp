/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddzuba <ddzuba@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 16:43:35 by hboichuk          #+#    #+#             */
/*   Updated: 2023/12/23 15:36:40 by ddzuba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ServerManager.hpp"

ServerManager::ServerManager(){}
ServerManager::~ServerManager(){}

//start all servers on ports, what specified in the config file
void    ServerManager::setupServers(std::vector<ServerConfiguration> servers)
{
    std::cout << std::endl;
    Logger::messageLog(B_YELLOW, CONSOLE_OUTPUT, "Initializing  Servers...");
    _servers = servers;
    char buf[INET_ADDRSTRLEN];
    bool    serverDub;
    for (std::vector<ServerConfiguration>::iterator it = _servers.begin(); it != _servers.end(); ++it)
    {
        serverDub = false;
        for (std::vector<ServerConfiguration>::iterator it2 = _servers.begin(); it2 != it; ++it2)
        {
            if (it2->getHost() == it->getHost() && it2->getPort() == it->getPort())
            {
                it->setFD(it2->getFd());
                serverDub = true;
            }
        }
        if (!serverDub)
            it->serverSetup();
        Logger::messageLog(B_GREEN, CONSOLE_OUTPUT, "Server Created: ServerName[%s] Host[%s] Port[%d]",it->getServerName().c_str(),
                       inet_ntop(AF_INET, &it->getHost(), buf, INET_ADDRSTRLEN), it->getPort());
    }
}

//start of all system
void    ServerManager::startServers()
{
    fd_set  recv_set_cpy;
    fd_set  write_set_cpy;
    int     select_ret;

    _max_fd = 0;
    initializeFdsSets();
    struct timeval timer;
    while (true)
    {
        timer.tv_sec = 1;
        timer.tv_usec = 0;
        recv_set_cpy = _read_fds_set;
        write_set_cpy = _write_fds_set;

        if ( (select_ret = select(_max_fd + 1, &recv_set_cpy, &write_set_cpy, NULL, &timer)) < 0 )
        {
            Logger::messageLog(B_YELLOW, CONSOLE_OUTPUT, "webserv: select error %s   Closing ....", strerror(errno));
            exit(1);
            continue ;
        }
        for (int i = 0; i <= _max_fd; ++i)
        {
            if (FD_ISSET(i, &recv_set_cpy) && _servers_map.count(i))
                getNewConnection(_servers_map.find(i)->second);
            else if (FD_ISSET(i, &recv_set_cpy) && _clients_map.count(i))
                readRequest(i, _clients_map[i]);
            else if (FD_ISSET(i, &write_set_cpy) && _clients_map.count(i))
            {
                int cgi_state = _clients_map[i].response.getCgiState(); // 0->NoCGI 1->CGI write/read to/from script 2-CGI read/write done
                if (cgi_state == 1 && FD_ISSET(_clients_map[i].response._cgi_conf.pipe_in[1], &write_set_cpy))
                    sendCgiBody(_clients_map[i], _clients_map[i].response._cgi_conf);
                else if (cgi_state == 1 && FD_ISSET(_clients_map[i].response._cgi_conf.pipe_out[0], &recv_set_cpy))
                    readCgiResponse(_clients_map[i], _clients_map[i].response._cgi_conf);
                else if ((cgi_state == 0 || cgi_state == 2)  && FD_ISSET(i, &write_set_cpy))
                    sendResponse(i, _clients_map[i]);
            }
        }
        checkTimeout();
    }
}

/*init for all sets of fds for listening requests*/
void    ServerManager::initializeFdsSets()
{
    FD_ZERO(&_read_fds_set);
    FD_ZERO(&_write_fds_set);

    // adds servers sockets to _recv_fd_pool set
    for(VECTOR<ServerConfiguration>::iterator it = _servers.begin(); it != _servers.end(); ++it)
    {
        //Now it calles listen() twice on even if two servers have the same host:port
        if (listen(it->getFd(), 512) == -1)
        {
            Logger::messageLog(B_RED, CONSOLE_OUTPUT, "webserv: listen error: %s   Closing....", strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (fcntl(it->getFd(), F_SETFL, O_NONBLOCK) < 0)
        {
            Logger::messageLog(B_RED, CONSOLE_OUTPUT, "webserv: fcntl error: %s   Closing....", strerror(errno));
            exit(EXIT_FAILURE);
        }
        addToSet(it->getFd(), _read_fds_set);
        _servers_map.insert(std::make_pair(it->getFd(), *it));
    }
    // at this stage _biggest_fd will belong to the last server created.
    _max_fd = _servers.back().getFd();
}

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
void    ServerManager::getNewConnection(ServerConfiguration &server)
{
    struct sockaddr_in      client_address;
    socklen_t               client_address_size = sizeof(client_address);
    int                     client_sock;
    Client                  new_client(server);
    char                    buf[INET_ADDRSTRLEN];

    if ( (client_sock = accept(server.getFd(), (struct sockaddr *)&client_address,
                               (socklen_t*)&client_address_size)) == -1)
    {
        Logger::messageLog(B_RED, CONSOLE_OUTPUT, "webserv: accept error %s", strerror(errno));
        return ;
    }
    Logger::messageLog(B_YELLOW, CONSOLE_OUTPUT, "New Connection From %s, Assigned Socket %d",inet_ntop(AF_INET, &client_address, buf, INET_ADDRSTRLEN), client_sock);

    addToSet(client_sock, _read_fds_set);

    if (fcntl(client_sock, F_SETFL, O_NONBLOCK) < 0)
    {
        Logger::messageLog(B_RED, CONSOLE_OUTPUT, "webserv: fcntl error %s", strerror(errno));
        removeFromSet(client_sock, _read_fds_set);
        close(client_sock);
        return ;
    }
    new_client.setSocket(client_sock);
    if (_clients_map.count(client_sock) != 0)
        _clients_map.erase(client_sock);
    _clients_map.insert(std::make_pair(client_sock, new_client));
}

/*Reads data from the client and provides it to the parser.
When the parser finishes or detects an error in the request,
the socket is transferred from the _read_fds_set to the _write_fds_set.
The response will be sent during the next iteration of the select() function.*/
void	ServerManager::readRequest(const int &i, Client &client)
{
	/*used to temporarily store data read from a client's socket*/
	char	buffer[40000]; //special define?
	/*keep track of the number of bytes that are read from 
	the client's socket during the read operation*/
	int     bytes_read = 0;
	
	bytes_read = read(i, buffer, 40000);
	if (bytes_read == 0)
	{
		Logger::messageLog(B_PINK, CONSOLE_OUTPUT, "webserv: Client %d Closed Connection", i);
		closeConnection(i);
		return ;
	}
	else if (bytes_read < 0)
	{
		Logger::messageLog(B_RED, CONSOLE_OUTPUT, "webserv: File Descriptor %d have read error %s", i, strerror(errno));
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
		Logger::messageLog(B_TURQUOISE, CONSOLE_OUTPUT, "Request: Recived From Socket %d, Method=<%s>  URI=<%s>"
                , i, client.request.getMethodStr().c_str(), client.request.getPath().c_str());
		client.buildResponse();
		if (client.response.getCgiState())
		{
			isReqBodyEmpty(client);
			addToSet(client.response._cgi_conf.pipe_in[1], _write_fds_set);
			addToSet(client.response._cgi_conf.pipe_out[0], _read_fds_set);
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
void    ServerManager::assignServer(Client &client)
{
    for (VECTOR<ServerConfiguration>::iterator it = _servers.begin();
         it != _servers.end(); ++it)
    {
        if (client.server.getHost() == it->getHost() &&
            client.server.getPort() == it->getPort() &&
            client.request.getServerName() == it->getServerName())
        {
            client.setServer(*it);
            return ;
        }
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
			Logger::messageLog(B_YELLOW, CONSOLE_OUTPUT, "Client %d timeout. Closing connection...", it->first);
			closeConnection(it->first);
			return ;
		}
		++it;
	}
}

//check this func!
void	ServerManager::sendResponse(const int &i, Client &client)
{
	int	bytes_num;
	std::string	response = client.response.getResponse();
	if (response.length() >= 40000)
		bytes_num = write(i, response.c_str(), 40000);
	else
		bytes_num = write(i, response.c_str(), response.length());

	if (bytes_num < 0)
	{
		Logger::messageLog(B_RED, CONSOLE_OUTPUT, "Wrong sendResponse %s", strerror(errno));
		closeConnection(i);
	}
	else if (bytes_num == 0 || (size_t) bytes_num == response.length())
	{
        Logger::messageLog(B_BLUE, CONSOLE_OUTPUT, "Response Sent To Socket %d, Stats=<%d>"
                , i, client.response.getCode());
		if (client.request.keepAlive() == false || client.request.errorCode() || client.response.getCgiState())
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
		client.response.cutResponse(bytes_num);
	}
}

/*CGI part*/
/*sent Request body to CGI script*/
void	ServerManager::sendCgiBody(Client &client, CGIConfig &cgi)
{
	/*keep track of the number of bytes sent by the write system call*/
	int	bytes_num;
	std::string	&req_body = client.request.getBody();

	if (req_body.length() == 0)
		bytes_num = 0;
	else if (req_body.length() >= 40000)
		bytes_num = write(cgi.pipe_in[1], req_body.c_str(), 40000);
	else
		bytes_num = write(cgi.pipe_in[1], req_body.c_str(), req_body.length());

	if (bytes_num < 0)
	{
		Logger::messageLog(B_RED, CONSOLE_OUTPUT, "Something wrong with sending %s", strerror(errno));
		removeFromSet(cgi.pipe_in[1], _write_fds_set);
		close(cgi.pipe_in[1]);
		close(cgi.pipe_out[1]);
		client.response.setErrorResponse(500);//check this line
		
	}
	else if (bytes_num == 0 || (size_t) bytes_num == req_body.length())
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
void	ServerManager::readCgiResponse(Client &client, CGIConfig &cgi)
{
	char	buffer[80000];
	int     bytes_num;
	
	bytes_num = read(cgi.pipe_out[0], buffer, 80000);
	if (bytes_num == 0)
	{
		removeFromSet(cgi.pipe_out[0], _read_fds_set);
		close(cgi.pipe_in[0]);
		close(cgi.pipe_out[0]);
        int		status;
		waitpid(cgi.getCGIpid(), &status, 0);
		/*child termination status*/
		if(WEXITSTATUS(status) != 0)
			client.response.setErrorResponse(502);//invalid response
		client.response.setCgiState(2);
		if (client.response._response_ready.find("HTTP/1.1") == std::string::npos)
			client.response._response_ready.insert(0, "HTTP/1.1 200 OK\r\n");
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
		client.response._response_ready.append(buffer,bytes_num);
		memset(buffer, 0, sizeof(buffer));
	}
}

void ServerManager::isReqBodyEmpty(Client &client)
{
    if (client.request.getBody().length() == 0)
    {
        STR tmp;
        std::fstream file;(client.response._cgi_conf.getCGIpath().c_str());
        std::stringstream ss;
        ss << file.rdbuf();
        tmp = ss.str();
        client.request.setBody(tmp);
    }
}
//{
//    std::string tmp;
//    std::fstream file;  // Changed from std::ifstream to std::fstream
//    std::stringstream string_stream;
//
//    if (client.request.getBody().empty())
//    {
//        file.open(client.response._cgi_conf.getCGIpath().c_str());  // Use .open() to open the file
//        string_stream << file.rdbuf();
//        tmp = string_stream.str();
//        client.request.setBody(tmp);
//        file.close();  // Close the file after reading
//    }
//}
