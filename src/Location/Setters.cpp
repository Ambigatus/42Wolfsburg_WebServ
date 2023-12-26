#include "../../include/Location.hpp"

void	Location::setPath(STR param)
{
	this->_path = param;
}

void	Location::setRootLocation(STR param)
{
	if (ConfigurationFile::getTypePath(param) != 2)
		throw ServerConfiguration::ErrorExeption("root of location. error here?");
    COUT << param << ENDL;
	this->_root = param;
}

void	Location::setMethods(VECTOR<STR> methods)
{
	this->_methods[0] = 0;
	this->_methods[1] = 0;
	this->_methods[2] = 0;
	this->_methods[3] = 0;
	this->_methods[4] = 0;

	for (size_t i = 0; i < methods.size(); i++)
	{
		if (methods[i] == "GET")
			this->_methods[0] = 1;
		else if (methods[i] == "POST")
			this->_methods[1] = 1;
		else if (methods[i] == "DELETE")
			this->_methods[2] = 1;
		else if (methods[i] == "PUT")
			this->_methods[3] = 1;
		else if (methods[i] == "HEAD")
			this->_methods[4] = 1;
		else
			throw ServerConfiguration::ErrorExeption("This method is not supported by server: " + methods[i]);
	}
}

void	Location::setAutoIndex(STR param)
{
	if (param == "on" || param == "off")
		this->_autoindex = (param == "on");
	else
		throw ServerConfiguration::ErrorExeption("Wrong autoindex status.");
}

void	Location::setIndexLocation(STR param)
{
	this->_index = param;
}

void	Location::setReturn(STR param)
{
	this->_return = param;
}

void	Location::setAlias(STR param)
{
	this->_alias = param;
}

void	Location::setCGIPath(VECTOR<STR> path)
{
	this->_cgi_path = path;
}

void	Location::setCGIExtension(VECTOR<STR> extension)
{
	this->_cgi_exten = extension;
}

void	Location::setMaxBodySize(STR param)
{
	size_t	body_size = 0;

	for (size_t i = 0; i < param.length(); i++)
	{
		if (param[i] < '0' || param[i] > '9')
			throw ServerConfiguration::ErrorExeption("Parameter 'client_max_body_size' must contain only numbers!");
	}
	if (!strToInt(param))
		throw ServerConfiguration::ErrorExeption("Wrong syntax in 'client_max_body_size'.");
	body_size = strToInt(param);
	this->_client_max_body_size = body_size;
}

void	Location::setMaxBodySize(size_t param)
{
	this->_client_max_body_size = param;
}
