#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "Webserv.hpp"

class ServerConfiguration;

class ConfigParser {
	private:
		VECTOR<ServerConfiguration>	    _servers;
		VECTOR<STR>	                    _server_config;
		size_t						    _nb_server;

	public:

		ConfigParser();
		~ConfigParser();

		int createCluster(const STR &config_file);

		void splitServers(STR &content);
		void removeComments(STR &content);
		void removeWhiteSpace(STR &content);
		size_t findStartServer(size_t start, STR &content);
		size_t findEndServer(size_t start, STR &content);
		void createServer(STR &config, ServerConfiguration &server);
		void checkServers();
		VECTOR<ServerConfiguration>	getServers();
		int	stringCompare(STR str1, STR str2, size_t pos);

		int print();

		public:
		class ErrorException : public std::exception
		{
			private:
				std::string _message;
			public:
				ErrorException(STR message) throw()
				{
					_message = "CONFIG PARSER ERROR: " + message;
				}
				virtual const char* what() const throw()
				{
					return (_message.c_str());
				}
				virtual ~ErrorException() throw() {}
		};
};

#endif