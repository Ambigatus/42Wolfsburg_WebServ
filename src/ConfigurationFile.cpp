#include "../inc/ConfigurationFile.hpp"

ConfigurationFile::ConfigurationFile()  { }

ConfigurationFile::ConfigurationFile(STR const path) : _path(path) { }

ConfigurationFile::~ConfigurationFile() { }


int ConfigurationFile::getTypePath(STR const path)
{
	struct stat	buffer;
	int			result;
	
	result = stat(path.c_str(), &buffer);
	if (result == 0)
	{
		if (buffer.st_mode & S_IFREG)
			return (1);
		else if (buffer.st_mode & S_IFDIR)
			return (2);
		else
			return (3);
	}
	else
		return (-1);
}

int	ConfigurationFile::checkFile(STR const path, int mode)
{
	return (access(path.c_str(), mode));
}

int ConfigurationFile::checkAccessToFile(STR const path, STR const index)
{
	if (getTypePath(index) == 1 && checkFile(index, 4) == 0)
		return (0);
	if (getTypePath(path + index) == 1 && checkFile(path + index, 4) == 0)
		return (0);
	return (-1);
}

STR	ConfigurationFile::readFile(STR path)
{
	if (path.empty() || path.length() == 0)
		return (NULL);
	std::ifstream config_file(path.c_str());
	if (!config_file || !config_file.is_open())
		return (NULL);

	std::stringstream stream_binding;
	stream_binding << config_file.rdbuf();
	return (stream_binding.str());
}

STR ConfigurationFile::getPath()
{
	return (this->_path);
}
