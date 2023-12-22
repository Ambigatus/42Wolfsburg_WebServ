#include "../../include/ConfigurationFile.hpp"

ConfigurationFile::ConfigurationFile() { }
ConfigurationFile::ConfigurationFile(STR const path) { this->_path = path;}
ConfigurationFile::~ConfigurationFile() { }

ConfigurationFile::ConfigurationFile(const ConfigurationFile &copy)
{
	if (this != &copy)
	{
		this->_path = copy._path;
	}
	return ;
}

ConfigurationFile &ConfigurationFile::operator=(const ConfigurationFile &copy)
{
	if (this != &copy)
	{
		this->_path = copy._path;
	}
	return *this;
}

int ConfigurationFile::getTypePath(STR const path)
{
	struct stat buffer;
	int		 result = stat(path.c_str(), &buffer);

	if (result == 0)
	{
		if(buffer.st_mode & S_IFREG)
			return(1);
		else if (buffer.st_mode & S_IFDIR)
			return(2);
		else
			return(3);
	}
	else
		return(-1);
}

int ConfigurationFile::checkConfigFile(STR const path, int mode)
{
	return(access(path.c_str(), mode));
}

int ConfigurationFile::checkIsFileReadable(STR const path, STR const index)
{
	if (getTypePath(index) == 1 && checkConfigFile(index, 4) == 0)
		return (0);
	if (getTypePath(path + index) == 1 && checkConfigFile(path + index, 4) == 0)
		return (0);
	return(-1);
}

STR ConfigurationFile::readFile(STR path)
{
	if (path.empty() || path.length() == 0)
		return (NULL);
	std::ifstream config_file(path.c_str());
	if (!config_file || !config_file.is_open())
		return(NULL);

	std::stringstream stream_binding;
	stream_binding << config_file.rdbuf();
	return (stream_binding.str());
}

STR ConfigurationFile::getPath() {return(this->_path);}
