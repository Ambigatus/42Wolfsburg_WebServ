#ifndef CONFIGURATIONPARSER_HPP
# define CONFIGURATIONPARSER_HPP

# include "Webserv.hpp"

class ServerConfiguration;

class ConfigurationParser {
	private:
			VECTOR<ServerConfiguration> _servers;			// Stores server configurations
			VECTOR<STR>					_server_config;		// Stores configuration strings for servers
			size_t						_nb_server;			// Number of servers

	public:
			ConfigurationParser();
			~ConfigurationParser();
//			ConfigurationParser(const ConfigurationParser &copy);
//			ConfigurationParser &operator=(const ConfigurationParser &copy);

			int		parseConfig(const STR &config_file); 					            // Parse the configuration file
			void	splitServers(STR &content);								// Split the configuration content into individual servers
			void	removeComments(STR &content);							// Remove comments from the configuration content
			void	removeWhiteSpace(STR &content);							// Remove whitespace from the configuration content
			size_t	findServerBegin(size_t start, STR &content);			// Find the start of a server
			size_t	findServerEnd(size_t start, STR &content);				// Find the end of a server
			void	createServer(STR &config, ServerConfiguration &server);	// Create a server configuration
			void	checkServers();											// Check for duplicated server configurations
//
			VECTOR<ServerConfiguration>	getServers();						 // Get the parsed server configurations
			int		strCompare(STR str1, STR str2, size_t pos);				 // Compare strings

			int		printConfig();											// Print server configurations
//
			public:
			class ErrorException : public std::exception
			{
				private:
					STR		_error_message;

				public:
					ErrorException(STR error_message) throw()
					{
						_error_message = "Configuration parser error: " + error_message;
					}
					virtual const char* what() const throw()
					{
						return (_error_message.c_str());
					}
					virtual ~ErrorException() throw() {}
			};
};

#endif
