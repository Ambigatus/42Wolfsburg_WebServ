#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

# include "Webserv.hpp"

class ConfigurationFile {
	private:
		STR	 _path;		// Path to the configuration file

	public:
		ConfigurationFile();
		ConfigurationFile(STR const path);
		~ConfigurationFile();
		ConfigurationFile(const ConfigurationFile &copy);
		ConfigurationFile &operator=(const ConfigurationFile &copy);

		static int getTypePath(STR const path);								// Check if the path is a file, folder, or something else
		static int checkConfigFile(STR const path, int mode);				// Check if the file exists and is accessible
		STR readFile(STR const path);										// Read the content of a file into a string
		static int checkIsFileReadable(STR const path, STR const index);	// Check he file, used for checking in ConfigurationParser

		STR getPath();	// Get the configuration file path
};

#endif
