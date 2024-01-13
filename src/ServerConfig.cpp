#include "../inc/ServerConfig.hpp"


static STR	serverParameters[] =	{  "server_name",
                                       "listen",
                                       "root",
                                       "index",
                                       "allow_methods",
                                       "client_body_buffer_size"};

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
	this->initErrorPages();
}

ServerConfiguration::~ServerConfiguration() { }

ServerConfiguration::ServerConfiguration(const ServerConfiguration &other)
{
	if (this != &other)
	{
		this->_server_name = other._server_name;
		this->_root = other._root;
		this->_host = other._host;
		this->_port = other._port;
		this->_client_max_body_size = other._client_max_body_size;
		this->_index = other._index;
		this->_error_pages = other._error_pages;
		this->_locations = other._locations;
		this->_listen_fd = other._listen_fd;
		this->_autoindex = other._autoindex;
		this->_server_address = other._server_address;

	}
	return ;
}

ServerConfiguration &ServerConfiguration::operator=(const ServerConfiguration & rhs)
{
	if (this != &rhs)
	{
		this->_server_name = rhs._server_name;
		this->_root = rhs._root;
		this->_port = rhs._port;
		this->_host = rhs._host;
		this->_client_max_body_size = rhs._client_max_body_size;
		this->_index = rhs._index;
		this->_error_pages = rhs._error_pages;
		this->_locations = rhs._locations;
		this->_listen_fd = rhs._listen_fd;
		this->_autoindex = rhs._autoindex;
		this->_server_address = rhs._server_address;
	}
	return (*this);
}

void ServerConfiguration::initErrorPages(void)
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
	_error_pages[505] = "";
}

void ServerConfiguration::setServerName(STR server_name)
{
	checkToken(server_name);
	this->_server_name = server_name;
}

void ServerConfiguration::setHost(STR param)
{
	checkToken(param);
	if (param == "localhost")
        param = "127.0.0.1";
	if (!isValidHost(param))
		throw ErrorException("Wrong syntax: host");
	this->_host = inet_addr(param.data());
}

void ServerConfiguration::setRoot(STR root)
{
	checkToken(root);
	if (ConfigurationFile::getTypePath(root) == 2)
	{
		this->_root = root;
		return ;
	}
	char dir[1024];
	getcwd(dir, 1024);
	STR full_root = dir + root;
	if (ConfigurationFile::getTypePath(full_root) != 2)
		throw ErrorException("Wrong syntax: root");
	this->_root = full_root;
}

void ServerConfiguration::setPort(STR parametr)
{
	unsigned int port;
	
	port = 0;
	checkToken(parametr);
	for (size_t i = 0; i < parametr.length(); i++)
	{
		if (!std::isdigit(parametr[i]))
			throw ErrorException("Wrong syntax: port");
	}
	port = strToInt((parametr));
	if (port < 1 || port > 65636)
		throw ErrorException("Wrong syntax: port");
	this->_port = (uint16_t) port;
}

void ServerConfiguration::setClientMaxBodySize(STR parametr)
{
	unsigned long body_size;
	
	body_size = 0;
	checkToken(parametr);
	for (size_t i = 0; i < parametr.length(); i++)
	{
		if (parametr[i] < '0' || parametr[i] > '9')
			throw ErrorException("Wrong syntax: client_max_body_size");
	}
	if (!strToInt(parametr))
		throw ErrorException("Wrong syntax: client_max_body_size");
	body_size = strToInt(parametr);
	this->_client_max_body_size = body_size;
}

void ServerConfiguration::setIndex(STR index)
{
	checkToken(index);
	this->_index = index;
}

void ServerConfiguration::setAutoindex(STR autoindex)
{
	checkToken(autoindex);
	if (autoindex != "on" && autoindex != "off")
		throw ErrorException("Wrong syntax: autoindex");
	if (autoindex == "on")
		this->_autoindex = true;
}

void ServerConfiguration::setErrorPages(std::vector<STR> &parametr)
{
	if (parametr.empty())
		return;
	if (parametr.size() % 2 != 0)
		throw ErrorException ("Error page initialization faled");
	for (size_t i = 0; i < parametr.size() - 1; i++)
	{
		for (size_t j = 0; j < parametr[i].size(); j++) {
			if (!std::isdigit(parametr[i][j]))
				throw ErrorException("Error code is invalid");
		}
		if (parametr[i].size() != 3)
			throw ErrorException("Error code is invalid");
		short code_error = strToInt(parametr[i]);
		if (statusCodeString(code_error)  == "Undefined" || code_error < 400)
			throw ErrorException ("Incorrect error code: " + parametr[i]);
		i++;
		STR path = parametr[i];
		checkToken(path);
		if (ConfigurationFile::getTypePath(path) != 2)
		{
			if (ConfigurationFile::getTypePath(this->_root + path) != 1)
				throw ErrorException ("Incorrect path for error page file: " + this->_root + path);
			if (ConfigurationFile::checkFile(this->_root + path, 0) == -1 || ConfigurationFile::checkFile(this->_root + path, 4) == -1)
				throw ErrorException ("Error page file :" + this->_root + path + " is not accessible");
		}
		std::map<short, STR>::iterator it = this->_error_pages.find(code_error);
		if (it != _error_pages.end())
			this->_error_pages[code_error] = path;
		else
			this->_error_pages.insert(std::make_pair(code_error, path));
	}
}

void ServerConfiguration::setLocation(STR path, std::vector<STR> parametr)
{
	Location new_location;
	std::vector<STR> methods;
	bool flag_methods = false;
	bool flag_autoindex = false;
	bool flag_max_size = false;
	int valid;

	new_location.setPath(path);
	for (size_t i = 0; i < parametr.size(); i++)
	{
		if (parametr[i] == "root" && (i + 1) < parametr.size())
		{
			if (!new_location.getRootLocation().empty())
				throw ErrorException("Root of location is duplicated");
			checkToken(parametr[++i]);
			if (ConfigurationFile::getTypePath(parametr[i]) == 2)
				new_location.setRootLocation(parametr[i]);
			else
				new_location.setRootLocation(this->_root + parametr[i]);
		}
		else if ((parametr[i] == "allow_methods" || parametr[i] == "methods") && (i + 1) < parametr.size())
		{
			if (flag_methods)
				throw ErrorException("Allow_methods of location is duplicated");
			std::vector<STR> methods;
			while (++i < parametr.size())
			{
				if (parametr[i].find(";") != std::string::npos)
				{
					checkToken(parametr[i]);
					methods.push_back(parametr[i]);
					break ;
				}
				else
				{
					methods.push_back(parametr[i]);
					if (i + 1 >= parametr.size())
						throw ErrorException("Token is invalid");
				}
			}
			new_location.setMethods(methods);
			flag_methods = true;
		}
		else if (parametr[i] == "autoindex" && (i + 1) < parametr.size())
		{
			if (path == "/cgi-bin")
				throw ErrorException("Parametr autoindex not allow for CGI");
			if (flag_autoindex)
				throw ErrorException("Autoindex of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setAutoindex(parametr[i]);
			flag_autoindex = true;
		}
		else if (parametr[i] == "index" && (i + 1) < parametr.size())
		{
			if (!new_location.getIndexLocation().empty())
				throw ErrorException("Index of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setIndexLocation(parametr[i]);
		}
		else if (parametr[i] == "return" && (i + 1) < parametr.size())
		{
			if (path == "/cgi-bin")
				throw ErrorException("Parametr return not allow for CGI");
			if (!new_location.getReturn().empty())
				throw ErrorException("Return of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setReturn(parametr[i]);
		}
		else if (parametr[i] == "alias" && (i + 1) < parametr.size())
		{
			if (path == "/cgi-bin")
				throw ErrorException("Parametr alias not allow for CGI");
			if (!new_location.getAlias().empty())
				throw ErrorException("Alias of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setAlias(parametr[i]);
		}
		else if (parametr[i] == "cgi_ext" && (i + 1) < parametr.size())
		{
			std::vector<STR> extension;
			while (++i < parametr.size())
			{
				if (parametr[i].find(";") != std::string::npos)
				{
					checkToken(parametr[i]);
					extension.push_back(parametr[i]);
					break ;
				}
				else
				{
					extension.push_back(parametr[i]);
					if (i + 1 >= parametr.size())
						throw ErrorException("Token is invalid");
				}
			}
			new_location.setCgiExtension(extension);
		}
		else if (parametr[i] == "cgi_path" && (i + 1) < parametr.size())
		{
			std::vector<STR> path;
			while (++i < parametr.size())
			{
				if (parametr[i].find(";") != std::string::npos)
				{
					checkToken(parametr[i]);
					path.push_back(parametr[i]);
					break ;
				}
				else
				{
					path.push_back(parametr[i]);
					if (i + 1 >= parametr.size())
						throw ErrorException("Token is invalid");
				}
				if (parametr[i].find("/python") == std::string::npos && parametr[i].find("/bash") == std::string::npos)
					throw ErrorException("cgi_path is invalid");
			}
			new_location.setCgiPath(path);
		}
		else if (parametr[i] == "client_max_body_size" && (i + 1) < parametr.size())
		{
			if (flag_max_size)
				throw ErrorException("Maxbody_size of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setMaxBodySize(parametr[i]);
			flag_max_size = true;
		}
		else if (i < parametr.size())
			throw ErrorException("Parametr in a location is invalid");
	}
	if (new_location.getPath() != "/cgi-bin" && new_location.getIndexLocation().empty())
		new_location.setIndexLocation(this->_index);
	if (!flag_max_size)
		new_location.setMaxBodySize(this->_client_max_body_size);
	valid = isValidLocation(new_location);
	if (valid == 1)
		throw ErrorException("Failed CGI validation");
	else if (valid == 2)
		throw ErrorException("Failed path in locaition validation");
	else if (valid == 3)
		throw ErrorException("Failed redirection file in locaition validation");
	else if (valid == 4)
		throw ErrorException("Failed alias file in locaition validation");
	this->_locations.push_back(new_location);
}

void	ServerConfiguration::setFd(int fd)
{
	this->_listen_fd = fd;
}

bool ServerConfiguration::isValidHost(STR host) const
{
	struct sockaddr_in sockaddr;
  	return (inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) ? true : false);
}

bool ServerConfiguration::isValidErrorPages()
{
	std::map<short, STR>::const_iterator it;
	for (it = this->_error_pages.begin(); it != this->_error_pages.end(); it++)
	{
		if (it->first < 100 || it->first > 599)
			return (false);
		if (ConfigurationFile::checkFile(getRoot() + it->second, 0) < 0 || ConfigurationFile::checkFile(getRoot() + it->second, 4) < 0)
			return (false);
	}
	return (true);
}

int ServerConfiguration::isValidLocation(Location &location) const
{
	if (location.getPath() == "/cgi-bin")
	{
		if (location.getCgiPath().empty() || location.getCgiExtension().empty() || location.getIndexLocation().empty())
			return (1);


		if (ConfigurationFile::checkFile(location.getIndexLocation(), 4) < 0)
		{
			STR path = location.getRootLocation() + location.getPath() + "/" + location.getIndexLocation();
			if (ConfigurationFile::getTypePath(path) != 1)
			{				
				STR root = getcwd(NULL, 0);
				location.setRootLocation(root);
				path = root + location.getPath() + "/" + location.getIndexLocation();
			}
			if (path.empty() || ConfigurationFile::getTypePath(path) != 1 || ConfigurationFile::checkFile(path, 4) < 0)
				return (1);
		}
		if (location.getCgiPath().size() != location.getCgiExtension().size())
			return (1);
		std::vector<STR>::const_iterator it;
		for (it = location.getCgiPath().begin(); it != location.getCgiPath().end(); ++it)
		{
			if (ConfigurationFile::getTypePath(*it) < 0)
				return (1);
		}
		std::vector<STR>::const_iterator it_path;
		for (it = location.getCgiExtension().begin(); it != location.getCgiExtension().end(); ++it)
		{
			STR tmp = *it;
			if (tmp != ".py" && tmp != ".sh" && tmp != "*.py" && tmp != "*.sh")
				return (1);
			for (it_path = location.getCgiPath().begin(); it_path != location.getCgiPath().end(); ++it_path)
			{
				STR tmp_path = *it_path;
				if (tmp == ".py" || tmp == "*.py")
				{
					if (tmp_path.find("python") != std::string::npos)
						location._ext_path.insert(std::make_pair(".py", tmp_path));
				}
				else if (tmp == ".sh" || tmp == "*.sh")
				{
					if (tmp_path.find("bash") != std::string::npos)
						location._ext_path[".sh"] = tmp_path;
				}
			}
		}
		if (location.getCgiPath().size() != location.getExtensionPath().size())
			return (1);
	}
	else
	{
		if (location.getPath()[0] != '/')
			return (2);
		if (location.getRootLocation().empty()) {
			location.setRootLocation(this->_root);
		}
		if (ConfigurationFile::checkAccessToFile(location.getRootLocation() + location.getPath() + "/",
                                                 location.getIndexLocation()))
			return (5);
		if (!location.getReturn().empty())
		{
			if (ConfigurationFile::checkAccessToFile(location.getRootLocation(), location.getReturn()))
				return (3);
		}
		if (!location.getAlias().empty())
		{
			if (ConfigurationFile::checkAccessToFile(location.getRootLocation(), location.getAlias()))
			 	return (4);
		}
	}
	return (0);
}

const STR &ServerConfiguration::getServerName()
{
	return (this->_server_name);
}

const STR &ServerConfiguration::getRoot()
{
	return (this->_root);
}

const bool &ServerConfiguration::getAutoindex()
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

const std::vector<Location> &ServerConfiguration::getLocations()
{
	return (this->_locations);
}

const std::map<short, STR> &ServerConfiguration::getErrorPages()
{
	return (this->_error_pages);
}

const STR &ServerConfiguration::getIndex()
{
	return (this->_index);
}

int   	ServerConfiguration::getFd()
{ 
	return (this->_listen_fd); 
}

const STR &ServerConfiguration::getPathErrorPage(short key)
{
	std::map<short, STR>::iterator it = this->_error_pages.find(key);
	if (it == this->_error_pages.end())
		throw ErrorException("Error_page does not exist");
	return (it->second);
}

const std::vector<Location>::iterator ServerConfiguration::getLocationKey(STR key)
{
	std::vector<Location>::iterator it;
	for (it = this->_locations.begin(); it != this->_locations.end(); it++)
	{
		if (it->getPath() == key)
			return (it);
	}
	throw ErrorException("Error: path to location not found");
}

void ServerConfiguration::checkToken(STR &parametr)
{
	size_t pos = parametr.rfind(';');
	if (pos != parametr.size() - 1)
		throw ErrorException("Token is invalid");
	parametr.erase(pos);
}

bool ServerConfiguration::checkLocaitons() const
{
	if (this->_locations.size() < 2)
		return (false);
	std::vector<Location>::const_iterator it1;
	std::vector<Location>::const_iterator it2;
	for (it1 = this->_locations.begin(); it1 != this->_locations.end() - 1; it1++) {
		for (it2 = it1 + 1; it2 != this->_locations.end(); it2++) {
			if (it1->getPath() == it2->getPath())
				return (true);
		}
	}
	return (false);
}

void	ServerConfiguration::setupServer(void)
{
	if ((_listen_fd = socket(AF_INET, SOCK_STREAM, 0) )  == -1 )
    {
		Logger::logMsg(RED, CONSOLE_OUTPUT, "webserv: socket error %s   Closing ....", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int option_value = 1;
    setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));
    memset(&_server_address, 0, sizeof(_server_address));
    _server_address.sin_family = AF_INET;
    _server_address.sin_addr.s_addr = _host;
    _server_address.sin_port = htons(_port);
    if (bind(_listen_fd, (struct sockaddr *) &_server_address, sizeof(_server_address)) == -1)
    {
		Logger::logMsg(RED, CONSOLE_OUTPUT, "webserv: bind error %s   Closing ....", strerror(errno));
        exit(EXIT_FAILURE);
    }
}