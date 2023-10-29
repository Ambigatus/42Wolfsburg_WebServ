#include "../../include/ServerConfiguration.hpp"

/*Setting the name for our server */
void	ServerConfiguration::setServerName(STR server_name)
{
	validationToken(server_name);
	this->_server_name = server_name;
}

/*This function is responsible for setting the host address for a server configuration.
Function validates and sets the host address for a server configuration, ensuring that 
it is in a valid format and either replacing "localhost" with "127.0.0.1" or validating 
the provided address. If the address is not valid, it raises an exception.*/
void	ServerConfiguration::setHost(STR param)
{
	validationToken(param);
	if (param == "localhost")
		param = "127.0.0.1";
	if (!checkHostValid(param))
		throw ErrorExeption("Error with the syntax in 'host'.");
	this->_host = inet_addr(param.data());
}

void	ServerConfiguration::setRoot(STR root)
{
	validationToken(root);
	//this part can be done only after finishing ConfigFile
}

/*This function is responsible for setting the port number for a server configuration.
Function validates and sets the port number for a server configuration, ensuring 
that it is in a valid format (numeric) and within the appropriate range. If the port 
number is not valid, it raises an exception. */
void	ServerConfiguration::setPort(STR param)
{
	size_t	port;

	port = 0;
	validationToken(param);
	for (size_t i = 0; i < param.length(); i++)
	{
		if (!std::isdigit(param[i]))
			throw ErrorExeption("Error with the syntax in 'port'.");
	}
	port = strToInt(param);
	if (port < 1 || port > 65636)
		throw ErrorExeption("Error with the syntax - port out of range(1 to 65636).");
	this->_port = (uint16_t) port;
}

/*This function is responsible for setting the maximum allowed client request body size 
for a server configuration. The function validates and sets the maximum client request 
body size for a server configuration, ensuring that it is in a valid format (numeric) 
and can be converted to an integer.*/
void	ServerConfiguration::setClientMaxBodySize(STR param)
{
	size_t	body_size;

	body_size = 0;
	validationToken(param);
	for (size_t i = 0; i < param.length(); i++)
	{
		if (param[i] < '0' || param[i] > '9')
			throw ErrorExeption("Error with the syntax - wrong Max Client Body Size.");
	}
	if (!strToInt(param))
		throw ErrorExeption("Error with the syntax - unconvertible parameter in Max Client Body Size.");
	body_size = strToInt(param);
	this->_client_max_body_size = body_size;
}

/*This function is responsible for setting the index file for a server configuration. */
void	ServerConfiguration::setIndex(STR index)
{
	validationToken(index);
	this->_index = index;
}

/*This function is responsible for setting the autoindex (directory listing) option for 
a server configuration. The function sets the autoindex option for a server configuration 
after ensuring that the provided autoindex value follows the specified syntax. It allows 
the server to enable or disable directory listing based on the "on" or "off" setting.*/
void	ServerConfiguration::setAutoIndex(STR autoindex)
{
	validationToken(autoindex);
	if (autoindex != "on" && autoindex != "off")
		throw ErrorExeption("Error with the syntax - autoindex must be 'on' or 'off'.");
	if (autoindex == "on")
		this->_autoindex = true;
}

void	ServerConfiguration::setErrorPages(VECTOR<STR> &param)
{
	if (param.empty())
		return;
	if (param.size() % 2 != 0)
		throw ErrorExeption("Error: error page initialization failed.");
	for (size_t i = 0; i < param.size() - 1; i++)
	{
		for (size_t o = 0; o < param[i].size(); o++)
		{
			if (!std::isdigit(param[i][o]))
				throw ErrorExeption("Error: error code is invalid.");
		}
		if (param[i].size() != 3)
			throw ErrorExeption("Error: error code must contain 3 digits.");
		short err_code = strToInt(param[i]);
		if (statusCodeList(err_code) == "Undefined" || err_code < 400)
			throw ErrorExeption("Error: this error code doesn't exist: " + param[i]);
		i++;
		STR path = param[i];
		validationToken(path);
		if (ConfigurationFile::getTypePath(path) != 2)
		{
			if (ConfigurationFile::getTypePath(this->_root + path) != 1)
				throw ErrorExeption("Error: incorrect path for error page: " + this->_root + path);
			if (ConfigurationFile::checkConfigFile(this->_root + path, 0) == -1 || ConfigurationFile::checkConfigFile(this->_root + path, 4) == -1)
				throw ErrorExeption("Error: can't get an access to error page file: " + this->_root + path);
		}
		MAP<short, STR>::iterator iter = this->_error_pages.find(err_code);
		if (iter != _error_pages.end())
			this->_error_pages[err_code] = path;
		else
			this->_error_pages.insert(std::make_pair(err_code, path));
	}
}

//setLocation here