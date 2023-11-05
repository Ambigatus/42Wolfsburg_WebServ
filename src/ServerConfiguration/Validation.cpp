#include "../../include/ServerConfiguration.hpp"

/*The function checks if the token ends with a semicolon (';'). The purpose of this 
function is to ensure that tokens are correctly formatted and terminate with a semicolon. 
If the token does not meet this criteria, it's considered invalid, and an exception is 
thrown.*/
void	ServerConfiguration::validationToken(STR &param)
{
	size_t buffer = param.rfind(';');
	if (buffer != param.size() - 1)
		throw ErrorExeption("This token is invalid.");
	param.erase(buffer);
}

/* This function is responsible for checking the validity of a host address. 
This function takes a host address as input and attempts to convert it to an IPv4 address 
using the inet_pton function. If the conversion is successful, it returns true, 
indicating that the host address is valid. If the conversion fails, it returns false, 
indicating that the host address is not a valid IPv4 address.*/
bool	ServerConfiguration::checkHostValid(STR host) const
{
	struct sockaddr_in	sockaddr;
	return (inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) ? true : false);
}

/*This function iterates through a map of error pages, where the keys are HTTP status 
codes and the values are paths to error pages. The function is used to ensure that 
error pages are correctly configured and that the associated files exist and are 
accessible.*/
bool	ServerConfiguration::checkErrPagesValid()
{
	MAP<short, STR>::const_iterator iter;
	for (iter = this->_error_pages.begin(); iter != this->_error_pages.end(); iter++)
	{
		if (iter->first < 100 || iter->first > 599)
			return false;
		if (ConfigurationFile::checkConfigFile(getRoot() + iter->second, 0) < 0 || ConfigurationFile::checkConfigFile(getRoot() + iter->second, 4) < 0)
			return false;
	}
	return true;
}

int	ServerConfiguration::checkLocationValid(Location &location) const
{
	if (location.getPath() == "/cgi-bin")
	{
		//need here CGI handler
	}
}
