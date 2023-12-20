/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboichuk <hboichuk@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 18:45:02 by hboichuk          #+#    #+#             */
/*   Updated: 2023/12/18 21:50:07 by hboichuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"

/* Stores all information about Client during Request-Response process*/
class Client 
{
	private:
		/*keep track of the time when the last message was received or sent by this client*/
		time_t	_last_msg_time;
		/*stores the socket descriptor associated with the client's network connection
		 It allows the server to send and receive data to and from the client */
		int	_client_fd_socket;
		/*store information about the client's network address(IP address and port number of the client)*/
		struct sockaddr_in  _client_ip_and_port;

		 
	public:
		Client();
		Client(ServerConfiguration &);
		Client(const Client &other);
		Client &operator=(const Client & rhs);
		~Client();

		/*needed objects*/
		Response				response;
		Request			        request;
		ServerConfiguration     server;

	//getters
		/*we need & here for incapsulation*/
		const int                 	&getSocket() const;
        const struct sockaddr_in  	&getIp() const;
        const time_t              	&getLastTime() const;
		const Request				&getRequest() const;
		
	//setter
		void                setSocket(int &);
        void                setIp(sockaddr_in &);
        void                setServer(ServerConfiguration &);
		
	//methods
		void	buildResponse();//build response with a client object
		void	updateTime();//method update for time
		void	clearClient();//clear Client
	
};

#endif
