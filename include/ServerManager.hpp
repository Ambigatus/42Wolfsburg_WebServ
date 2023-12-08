#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include "Webserv.hpp"//libraries
#include "Client.hpp"//all data for Client part
#include "Response.hpp"//all data for Response part


//class for init server
class ServerManager 
{
	public:
		ServerManager();
		~ServerManager();
		void	startServers();
		void	setupServers(std::vector<ServerConfiguration>);

	private:
	//variables
		/*list of configuration of all servers*/
		std::vector<ServerConfiguration>	_servers_config;

		/*keep track of which server configuration corresponds to which file 
		descriptor, especially for listening sockets.*/
		std::map<int, ServerConfiguration> _servers_map;

		/* max_fd - keep track of the highest file descriptor for select method
		and efficiently monitoring multiple file descriptors when 
		waiting for data on different connections*/
		int	_max_fd;

	/*part for listen_fds - I can replace it with ServerConfig Class if we need it*/
		std::map<int, int> _listen_fds;      // Map listening sockets to server IDs
    	std::map<int, Client> _clients_map;
		int getListenFd(int serverId);
	/*end of this part*/

		fd_set	_read_fds; // sockets that the server is interested in for reading
		fd_set	_write_fds; // sockets that the server is interested in for writing

	//methods
		/* init two sets for listening requests(reading and writing FDs)*/
		void	initializeFdsSets();
		void	addToSet(const int , fd_set &);
		void	removeFromSet(const int , fd_set &);
		void	getNewConnection(ServerConfiguration &);
		void	readRequest(const int &, Client &);
		void    isReqBodyEmpty(Client &);
		void	assignServer(Client &);
		void	checkTimeout();
		void	sendResponse(const int &, Client &);
		/*CGI part*/
		void	sendCgiBody(Client &, CGIConfig &);
		void	readCgiResponse(Client &, CGIConfig &);
		void	closeConnection(const int);

};

#endif
