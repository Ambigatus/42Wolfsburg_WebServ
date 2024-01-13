#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include "Webserv.hpp"
#include "Client.hpp"
#include "Response.hpp"

class ServerManager
{
    public:                 
        ServerManager();
        ~ServerManager();
        void    setupServers(std::vector<ServerConfiguration>);
        void    runServers();
        
    private:
        VECTOR<ServerConfiguration>     _servers;
        MAP<int, ServerConfiguration>   _servers_map;
        MAP<int, Client>                _clients_map;
        fd_set                          _read_fd_set;
        fd_set                          _write_fd_set;
        int                             _max_fd;

        void acceptNewConnection(ServerConfiguration &);
        void checkTimeout();
        void initializeSets();
        void readRequest(const int &, Client &);
        void handleReqBody(Client &);
        void sendResponse(const int &, Client &);
        void sendCgiBody(Client &, CGIConfig &);
        void readCgiResponse(Client &, CGIConfig &);
        void closeConnection(const int);
        void assignServer(Client &);
        void addToSet(const int , fd_set &);
        void removeFromSet(const int , fd_set &);
};


#endif
