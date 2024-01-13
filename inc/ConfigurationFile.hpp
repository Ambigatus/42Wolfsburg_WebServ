#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "Webserv.hpp"

class ConfigurationFile {
	private:
		STR		        _path;

	public:
		ConfigurationFile();
		ConfigurationFile(STR const path);
		~ConfigurationFile();

		static int      getTypePath(std::string const path);
		static int      checkFile(std::string const path, int mode);
		STR	            readFile(std::string path);
		static int      checkAccessToFile(std::string const path, std::string const index);

		STR             getPath();
};

#endif