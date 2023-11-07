#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include "Webserv.hpp"//libraries
#include "Client.hpp"//all data for Client part


//class for init server
class ServerManager 
{
	private:
	//variables
		/*list of configuration of all servers*/
		std::vector<ServerConfig>	_servers_config;

		/*keep track of which server configuration corresponds to which file 
		descriptor, especially for listening sockets.*/
		std::map<int, ServerConfig> _servers_map;

		/* max_fd - keep track of the highest file descriptor for select method
		and efficiently monitoring multiple file descriptors when 
		waiting for data on different connections*/
		int	_max_fd;

	/*part for listen_fds - I can replace it with ServerConfig Class if we need it*/
		std::map<int, int> _listen_fds;      // Map listening sockets to server IDs
    	std::map<int, Client> _clients_map;
		int getListenFd(int serverId);
	/*end of this part*/

	//methods
		/* init two sets for listening requests(reading and writing FDs)*/
		void	initializeFdsSets();
		void	addToSet(const int , fd_set &);
		void	removeFromSet(const int , fd_set &);
		void	getNewConnection(ServerConfig &);
		void	readRequest(const int &, Client &);
		void	assignServer(Client &);

		void	closeConnection(const int);

	public:
		ServerManager();
		~ServerManager();
		void	startServers();
		void	setupServers(std::vector<ServerConfig>);

}

#endif