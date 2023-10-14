#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include "Webserv.hpp"//libraries


//class for init server

class ServerManager 
{
	private:
		std::vector<ServerConfig>	_servers;

	public:
		ServerManager();
		~ServerManager();
		void	startServers();
		void	setupServers(std::vector<ServerConfig> servers);

}

#endif