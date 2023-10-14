#ifndef SERVERCONFIGURATION
# define SERVERCONFIGURATION

# include "Webserv.hpp"

static STR	serverParameters[] =	{"server_name",
									"listen",
									"root",
									"index",
									"allow_methods",
									"client_body_buffer_size"};

class Location;

class ServerConfiguration
{
	private:
		STR									_server_name;
		STR									_index;
		STR									_root;
		uint16_t							_port;
		in_addr_t							_host;
		size_t								_client_max_body_size;
		bool								_autoindex;
		MAP<short, STR>						_error_pages;
		VECTOR<Location>					_locations;
		struct sockaddr_in					_server_address;
		int									_listen_fd;

	public:
		ServerConfiguration();
		ServerConfiguration(const ServerConfiguration &copy);
		ServerConfiguration &operator=(const ServerConfiguration &copy);
		~ServerConfiguration();

		void								setServerName(STR server_name);
		void								setHost(STR param);
		void								setRoot(STR root);
		void								setFD(int);
		void								setPort(STR param);
		void								setClientMaxBodySize(STR param);
		void								setErrorPages(VECTOR<STR> &param);
		void								setIndex(STR index);
		void								setLocation(STR locationName, VECTOR<STR> param);
		void								setAutoIndex(STR autoindex);

		bool								checkHostValid(STR host) const;
		bool								checkErrPagesValid();
		int									checkLocationValid(Location &location) const;
		void								createErrorPages(void);

		const STR							&getServerName();
		const uint16_t						&getPort();
		const in_addr_t						&getHost();
		const size_t						&getClientMaxBodySize();
		const VECTOR<Location>				&getLocations();
		const STR							&getRoot();
		const MAP<short, STR>				&getErrorPages();
		const STR							&getIndex();
		const bool							&getAotoIndex();
		const STR							&getPathErrorPage(short key);
		const VECTOR<Location>::iterator	getLocationKey(STR key);


};

#endif