#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Client
{
    public:
        Client();
        Client(const Client &other);
        Client(ServerConfiguration &);
        Client &operator=(const Client & rhs);
        ~Client();

        const int                   &getSocket() const;
        const struct sockaddr_in    &getAddress() const;
        const Request               &getRequest() const;
        const time_t                &getLastTime() const;

        void                        setSocket(int &);
        void                        setAddress(sockaddr_in &);
        void                        setServer(ServerConfiguration &);
        void                        buildResponse();
        void                        updateTime();

        void                        clearClient();
        Response                    response;
        Request                     request;
        ServerConfiguration                server;
    private:
        int                         _client_socket;
        struct sockaddr_in          _client_address;
        time_t                      _last_msg_time;
};


#endif // CLIENT_HPP