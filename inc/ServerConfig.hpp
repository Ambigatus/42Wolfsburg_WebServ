#ifndef SERVERCONFIGURATION_HPP
# define SERVERCONFIGURATION_HPP

# include "Webserv.hpp"

class Location;

class ServerConfiguration
{
	private:
		uint16_t						_port;
		in_addr_t						_host;
		STR						        _server_name;
		STR						        _root;
		unsigned long					_client_max_body_size;
		STR						        _index;
		bool							_autoindex;
		MAP<short, STR>	                _error_pages;
		VECTOR<Location> 			    _locations;
        struct sockaddr_in 				_server_address;
        int     						_listen_fd;

	public:
		ServerConfiguration();
		~ServerConfiguration();
		ServerConfiguration(const ServerConfiguration &other);
		ServerConfiguration &operator=(const ServerConfiguration & rhs);

		void initErrorPages(void);

		void setServerName(STR server_name);
		void setHost(STR param);
		void setRoot(STR root);
		void setFd(int);
		void setPort(STR parametr);
		void setClientMaxBodySize(STR param);
		void setErrorPages(std::vector<STR> &param);
		void setIndex(STR index);
		void setLocation(STR nameLocation, std::vector<STR> param);
		void setAutoindex(STR autoindex);

		bool isValidHost(STR host) const;
		bool isValidErrorPages();
		int isValidLocation(Location &location) const;

		const STR &getServerName();
		const uint16_t &getPort();
		const in_addr_t &getHost();
		const size_t &getClientMaxBodySize();
		const VECTOR<Location> &getLocations();
		const STR &getRoot();
		const MAP<short, STR> &getErrorPages();
		const STR &getIndex();
		const bool &getAutoindex();
		const STR &getPathErrorPage(short key);
		const VECTOR<Location>::iterator getLocationKey(STR key);

		static void checkToken(STR &parametr);
		bool        checkLocaitons() const;

		public:
		class ErrorException : public std::exception
		{
			private:
				STR _message;
			public:
				ErrorException(STR message) throw()
				{
					_message = "SERVER CONFIG ERROR: " + message;
				}
				virtual const char* what() const throw()
				{
					return (_message.c_str());
				}
				virtual ~ErrorException() throw() {}
		};

		void	setupServer();
        int     getFd();

};

#endif
