#ifndef SERVERCONFIGURATION_HPP
# define SERVERCONFIGURATION_HPP

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
        bool                                checkLocationDup(void);

		const STR							&getServerName();
		const uint16_t						&getPort();
		const in_addr_t						&getHost();
		const size_t						&getClientMaxBodySize();
		const VECTOR<Location>				&getLocations();
		const STR							&getRoot();
		const MAP<short, STR>				&getErrorPages();
		const STR							&getIndex();
		const bool							&getAutoIndex();
		const STR							&getPathErrorPage(short key);
		const VECTOR<Location>::iterator	getLocationKey(STR key);

		static void							validationToken(STR &param);
		bool								validationLocations() const;

		public:
			class ErrorExeption : public std::exception
			{
				private:
					STR		_error_message;

				public:
					explicit ErrorExeption(STR error_message) throw()
					{
						_error_message = "Server configuration error: " + error_message;
					}
					virtual const char* what() const throw()
					{
						return (_error_message.c_str());
					}
					virtual ~ErrorExeption() throw() {}
			};
			
		void								serverSetup();
		int									getFd();
};

#endif
