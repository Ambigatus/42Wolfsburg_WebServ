#include "../../include/ServerConfiguration.hpp"

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
    return *this;
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
