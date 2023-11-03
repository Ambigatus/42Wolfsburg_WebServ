/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboichuk <hboichuk@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 18:46:04 by hboichuk          #+#    #+#             */
/*   Updated: 2023/10/29 12:37:49 by hboichuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client()
{
    _last_msg_time = time(NULL);
}

Client::Client(const Client &other)
{
    if (this != &other)
    {
        this->_client_fd_socket = other._client_fd_socket;
        this->_client_ip_and_port = other._client_ip_and_port;
        this->_last_msg_time = other._last_msg_time;
        //response
        //server config
        //request
    }
    return ;
}

Client::Client &operator=(const Client & rhs)
{
    if (this != &rhs)
    {
        this->_client_fd_socket = rhs._client_fd_socket;
        this->_client_ip_and_port = rhs._client_ip_and_port;
        this->_last_msg_time = rhs._last_msg_time;
        //response
        //server config
        //request
    }
    return (*this);
}

Client::Client(ServerConfig &server)
{
    //doesn't finished
}

Client::~Client() {}

//methods

//getters
const int   Client::&getSocket() const
{
    return (_client_fd_socket);
}

const struct sockaddr_in  Client::&getIp() const
{
    return (_client_ip_and_port);
}

const time_t    Client::&getLastTime() const
{
    return (_last_msg_time);
}

//setters
void    Client::setSocket(int &socket)
{
    _client_fd_socket = socket;
}

void    Client::setIp(sockaddr_in &ip)
{
    _client_ip_and_port = ip;
}
