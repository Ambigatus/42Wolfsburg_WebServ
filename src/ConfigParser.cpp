#include "../inc/ConfigParser.hpp"

ConfigParser::ConfigParser()
{
	this->_nb_server = 0;
}

ConfigParser::~ConfigParser() { }

int ConfigParser::print()
{
	COUT << "------------- Config -------------" << ENDL;
	for (size_t i = 0; i < _servers.size(); i++)
	{
		COUT << "Server #" << i + 1 << ENDL;
		COUT << "Server name: " << _servers[i].getServerName() << ENDL;
		COUT << "Host: " << _servers[i].getHost() << ENDL;
		COUT << "Root: " << _servers[i].getRoot() << ENDL;
		COUT << "Index: " << _servers[i].getIndex() << ENDL;
		COUT << "Port: " << _servers[i].getPort() << ENDL;
		COUT << "Max BSize: " << _servers[i].getClientMaxBodySize() << ENDL;
		COUT << "Error pages: " << _servers[i].getErrorPages().size() << ENDL;
		std::map<short, STR>::const_iterator it = _servers[i].getErrorPages().begin();
		while (it != _servers[i].getErrorPages().end())
		{
			COUT << (*it).first << " - " << it->second << ENDL;
			++it;
		}
		COUT << "Locations: " << _servers[i].getLocations().size() << ENDL;
		std::vector<Location>::const_iterator itl = _servers[i].getLocations().begin();
		while (itl != _servers[i].getLocations().end())
		{
			COUT << "name location: " << itl->getPath() << ENDL;
			COUT << "methods: " << itl->getPrintMethods() << ENDL;
			COUT << "index: " << itl->getIndexLocation() << ENDL;
			if (itl->getCgiPath().empty())
			{
				COUT << "root: " << itl->getRootLocation() << ENDL;
				if (!itl->getReturn().empty())
					COUT << "return: " << itl->getReturn() << ENDL;
				if (!itl->getAlias().empty())
					COUT << "alias: " << itl->getAlias() << ENDL;
			}
			else
			{
				COUT << "cgi root: " << itl->getRootLocation() << ENDL;
				COUT << "sgi_path: " << itl->getCgiPath().size() << ENDL;
				COUT << "sgi_ext: " << itl->getCgiExtension().size() << ENDL;
			}
			++itl;
		}
		itl = _servers[i].getLocations().begin();
		COUT << "-----------------------------" << ENDL;
	}
	return (0);
}

int ConfigParser::createCluster(const STR &config_file)
{
	STR		content;
	ConfigurationFile		file(config_file);

	if (file.getTypePath(file.getPath()) != 1)
		throw ErrorException("File is invalid");
	if (file.checkFile(file.getPath(), 4) == -1)
		throw ErrorException("File is not accessible");
	content = file.readFile(config_file);
	if (content.empty())
		throw ErrorException("File is empty");
	removeComments(content);
	removeWhiteSpace(content);
	splitServers(content);
	if (this->_server_config.size() != this->_nb_server)
		throw ErrorException("Something with size");
	for (size_t i = 0; i < this->_nb_server; i++)
	{
		ServerConfiguration server;
		createServer(this->_server_config[i], server);
		this->_servers.push_back(server);
	}
	if (this->_nb_server > 1)
		checkServers();
	return (0);
}

void ConfigParser::removeComments(STR &content)
{
	size_t pos;

	pos = content.find('#');
	while (pos != std::string::npos)
	{
		size_t pos_end;
		pos_end = content.find('\n', pos);
		content.erase(pos, pos_end - pos);
		pos = content.find('#');
	}
}

void ConfigParser::removeWhiteSpace(STR &content)
{
	size_t	i = 0;

	while (content[i] && isspace(content[i]))
		i++;
	content = content.substr(i);
	i = content.length() - 1;
	while (i > 0 && isspace(content[i]))
		i--;
	content = content.substr(0, i + 1);
}

void ConfigParser::splitServers(STR &content)
{
	size_t start = 0;
	size_t end = 1;

	if (content.find("server", 0) == std::string::npos)
		throw ErrorException("Server did not find");
	while (start != end && start < content.length())
	{
		start = findStartServer(start, content);
		end = findEndServer(start, content);
		if (start == end)
			throw ErrorException("problem with scope");
		this->_server_config.push_back(content.substr(start, end - start + 1));
		this->_nb_server++;
		start = end + 1;
	}
}

size_t ConfigParser::findStartServer (size_t start, STR &content)
{
	size_t i;

	for (i = start; content[i]; i++)
	{
		if (content[i] == 's')
			break ;
		if (!isspace(content[i]))
			throw  ErrorException("Wrong character out of server scope{}");
	}
	if (!content[i])
		return (start);
	if (content.compare(i, 6, "server") != 0)
		throw ErrorException("Wrong character out of server scope{}");
	i += 6;
	while (content[i] && isspace(content[i]))
		i++;
	if (content[i] == '{')
		return (i);
	else
		throw  ErrorException("Wrong character out of server scope{}");

}

size_t ConfigParser::findEndServer (size_t start, STR &content)
{
	size_t	i;
	size_t	scope;
	
	scope = 0;
	for (i = start + 1; content[i]; i++)
	{
		if (content[i] == '{')
			scope++;
		if (content[i] == '}')
		{
			if (!scope)
				return (i);
			scope--;
		}
	}
	return (start);
}

std::vector<STR> splitParametrs(STR line, STR sep)
{
	std::vector<STR>	str;
	std::string::size_type		start, end;

	start = end = 0;
	while (1)
	{
		end = line.find_first_of(sep, start);
		if (end == std::string::npos)
			break;
		STR tmp = line.substr(start, end - start);
		str.push_back(tmp);
		start = line.find_first_not_of(sep, end);
		if (start == std::string::npos)
			break;
	}
	return (str);
}

void ConfigParser::createServer(STR &config, ServerConfiguration &server)
{
	std::vector<STR>	parametrs;
	std::vector<STR>	error_codes;
	int		flag_loc = 1;
	bool	flag_autoindex = false;
	bool	flag_max_size = false;

	parametrs = splitParametrs(config += ' ', STR(" \n\t"));
	if (parametrs.size() < 3)
		throw  ErrorException("Failed server validation");
	for (size_t i = 0; i < parametrs.size(); i++)
	{
		if (parametrs[i] == "listen" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (server.getPort())
				throw  ErrorException("Port is duplicated");
			server.setPort(parametrs[++i]);
		}
		else if (parametrs[i] == "location" && (i + 1) < parametrs.size())
		{
			STR	path;
			i++;
			if (parametrs[i] == "{" || parametrs[i] == "}")
				throw  ErrorException("Wrong character in server scope{}");
			path = parametrs[i];
			std::vector<STR> codes;
			if (parametrs[++i] != "{")
				throw  ErrorException("Wrong character in server scope{}");
			i++;
			while (i < parametrs.size() && parametrs[i] != "}")
				codes.push_back(parametrs[i++]);
			server.setLocation(path, codes);
			if (i < parametrs.size() && parametrs[i] != "}")
				throw  ErrorException("Wrong character in server scope{}");
			flag_loc = 0;
		}
		else if (parametrs[i] == "host" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (server.getHost())
				throw  ErrorException("Host is duplicated");
			server.setHost(parametrs[++i]);
		}
		else if (parametrs[i] == "root" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (!server.getRoot().empty())
				throw  ErrorException("Root is duplicated");
			server.setRoot(parametrs[++i]);
		}
		else if (parametrs[i] == "error_page" && (i + 1) < parametrs.size() && flag_loc)
		{
			while (++i < parametrs.size())
			{
				error_codes.push_back(parametrs[i]);
				if (parametrs[i].find(';') != std::string::npos)
					break ;
				if (i + 1 >= parametrs.size())
					throw ErrorException("Wrong character out of server scope{}");
			}
		}
		else if (parametrs[i] == "client_max_body_size" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (flag_max_size)
				throw  ErrorException("Client_max_body_size is duplicated");
			server.setClientMaxBodySize(parametrs[++i]);
			flag_max_size = true;
		}
		else if (parametrs[i] == "server_name" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (!server.getServerName().empty())
				throw  ErrorException("Server_name is duplicated");
			server.setServerName(parametrs[++i]);
		}
		else if (parametrs[i] == "index" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (!server.getIndex().empty())
				throw  ErrorException("Index is duplicated");
			server.setIndex(parametrs[++i]);
		}
		else if (parametrs[i] == "autoindex" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (flag_autoindex)
				throw ErrorException("Autoindex of server is duplicated");
			server.setAutoindex(parametrs[++i]);
			flag_autoindex = true;
		}
		else if (parametrs[i] != "}" && parametrs[i] != "{")
		{
			if (!flag_loc)
				throw  ErrorException("Parametrs after location");
			else
				throw  ErrorException("Unsupported directive");
		}
	}
	if (server.getRoot().empty())
		server.setRoot("/;");
	if (server.getHost() == 0)
		server.setHost("localhost;");
	if (server.getIndex().empty())
		server.setIndex("index.html;");
	if (ConfigurationFile::checkAccessToFile(server.getRoot(), server.getIndex()))
		throw ErrorException("Index from config file not found or unreadable");
	if (server.checkLocaitons())
		throw ErrorException("Locaition is duplicated");
	if (!server.getPort())
		throw ErrorException("Port not found");
	server.setErrorPages(error_codes);
	if (!server.isValidErrorPages())
		throw ErrorException("Incorrect path for error page or number of error");
}

int	ConfigParser::stringCompare(STR str1, STR str2, size_t pos)
{
	size_t	i;

	i = 0;
	while (pos < str1.length() && i < str2.length() && str1[pos] == str2[i])
	{
		pos++;
		i++;
	}
	if (i == str2.length() && pos <= str1.length() && (str1.length() == pos || isspace(str1[pos])))
		return (0);
	return (1);
}

void ConfigParser::checkServers()
{
	std::vector<ServerConfiguration>::iterator it1;
	std::vector<ServerConfiguration>::iterator it2;

	for (it1 = this->_servers.begin(); it1 != this->_servers.end() - 1; it1++)
	{
		for (it2 = it1 + 1; it2 != this->_servers.end(); it2++)
		{
			if (it1->getPort() == it2->getPort() && it1->getHost() == it2->getHost() && it1->getServerName() == it2->getServerName())
				throw ErrorException("Failed server validation");
		}
	}
}

std::vector<ServerConfiguration>	ConfigParser::getServers()
{
	return (this->_servers);
}