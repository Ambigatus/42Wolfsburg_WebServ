#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include "Webserv.hpp"//libraries


//class for init server
class ServerManager 
{
	private:
	//variables
		/*list of configuration of all servers*/
		std::vector<ServerConfig>	_servers_config;

		/* max_fd - keep track of the highest file descriptor for select method
		and efficiently monitoring multiple file descriptors when 
		waiting for data on different connections*/
		int	max_fd;

	//methods
		/* init two sets for listening requests(reading and writing FDs)*/
		void	initializeFdsSets(); 

	public:
		ServerManager();
		~ServerManager();
		void	startServers();
		void	setupServers(std::vector<ServerConfig>);

}

#endif