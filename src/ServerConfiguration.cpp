#include "../include/ServerConfiguration.hpp"

/*The ServerConfiguration class constructor initializes various properties of a server
configuration object. It sets default values for these properties, such as port number, 
host, server name, root directory, maximum client body size, default index file, 
listening file descriptor, and autoindex option. Additionally, it creates error pages 
for the server configuration.*/
ServerConfiguration::ServerConfiguration()
{
	this->_port = 0;
	this->_host = 0;
	this->_server_name = "";
	this->_root = "";
	this->_client_max_body_size = MAX_CONTENT_LENGTH;
	this->_index = "";
	this->_listen_fd = 0;
	this->_autoindex = false;
	this->createErrorPages();
}

ServerConfiguration::~ServerConfiguration(){}

/*The ServerConfiguration class copy constructor is responsible for creating a new 
instance of the class as a copy of an existing ServerConfiguration object (copy). */
ServerConfiguration::ServerConfiguration(const ServerConfiguration &copy)
{
	if (this != &copy)
	{
		this->_server_name = copy._server_name;
		this->_port = copy._port;
		this->_root = copy._root;
		this->_host = copy._host;
		this->_client_max_body_size = copy._client_max_body_size;
		this->_index = copy._index;
		this->_error_pages = copy._error_pages;
		this->_locations = copy._locations;
		this->_listen_fd = copy._listen_fd;
		this->_autoindex = copy._autoindex;
		this->_server_address = copy._server_address;
	}
	return ;
}

/*The ServerConfiguration class copy assignment operator (operator=) is responsible for 
assigning the properties of another ServerConfiguration object (copy) to the 
current object.*/
ServerConfiguration &ServerConfiguration::operator=(const ServerConfiguration &copy)
{
	if (this != &copy)
	{
		this->_server_name = copy._server_name;
		this->_port = copy._port;
		this->_root = copy._root;
		this->_host = copy._host;
		this->_client_max_body_size = copy._client_max_body_size;
		this->_index = copy._index;
		this->_error_pages = copy._error_pages;
		this->_locations = copy._locations;
		this->_listen_fd = copy._listen_fd;
		this->_autoindex = copy._autoindex;
		this->_server_address = copy._server_address;
	}
}

/*Creating a blank for error pages */
void	ServerConfiguration::createErrorPages(void)
{
	_error_pages[301] = "";
	_error_pages[302] = "";
	_error_pages[400] = "";
	_error_pages[401] = "";
	_error_pages[402] = "";
	_error_pages[403] = "";
	_error_pages[404] = "";
	_error_pages[405] = "";
	_error_pages[406] = "";
	_error_pages[500] = "";
	_error_pages[501] = "";
	_error_pages[502] = "";
	_error_pages[503] = "";
	_error_pages[505] = "";
}
/*Chekers and validators*/
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
		//we need to add here another part of code after finishing configuration file
		//We need to check config file for permissions on the file located at the combined root and file path
	}
	return true;
}

/*Setters*/
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
