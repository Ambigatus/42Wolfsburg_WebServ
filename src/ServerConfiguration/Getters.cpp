#include "../../include/Webserv.hpp"

const STR &ServerConfiguration::getServerName()
{
	return (this->_server_name);
}

const STR &ServerConfiguration::getRoot()
{
	return (this->_root);
}

const bool &ServerConfiguration::getAotoIndex()
{
	return (this->_autoindex);
}

const in_addr_t &ServerConfiguration::getHost()
{
	return (this->_host);
}

const uint16_t &ServerConfiguration::getPort()
{
	return (this->_port);
}

const size_t &ServerConfiguration::getClientMaxBodySize()
{
	return (this->_client_max_body_size);
}

const VECTOR<Location> &ServerConfiguration::getLocations()
{
	return (this->_locations);
}

const MAP<short, STR> &ServerConfiguration::getErrorPages()
{
	return (this->_error_pages);
}

const STR &ServerConfiguration::getIndex()
{
	return (this->_index);
}

int	ServerConfiguration::getFd()
{
	return (this->_listen_fd);
}

const STR &ServerConfiguration::getPathErrorPage(short key)
{
	MAP<short, STR>::iterator iter = this->_error_pages.find(key);
	if (iter == this->_error_pages.end())
		throw ErrorExeption("Warning: This error page didn't exist!");
	return (iter->second);
}

const VECTOR<Location>::iterator ServerConfiguration::getLocationKey(STR key)
{
	VECTOR<Location>::iterator iter;
	for (iter = this->_locations.begin(); iter != this->_locations.end(); iter++)
	{
		if (iter->getPath() == key)
			return (iter);
	}
	throw ErrorExeption("ERROR: location path not found.");
}
