#include "../../include/ConfigurationFile.hpp"

ConfigurationParser::ConfigurationParser() { this->_nb_server = 0;}
ConfigurationParser::~ConfigurationParser() {}

ConfigurationParser::ConfigurationParser(const ConfigurationParser &copy)
{
	if (this != &copy)
	{
		this->_nb_server = copy._nb_server;
	}
	return ;
}

ConfigurationParser &ConfigurationParser::operator=(const ConfigurationParser &copy)
{
	if (this != &copy)
	{
		this->_nb_server = copy._nb_server;
	}
	return *this;
}

int ConfigurationParser::parseConfig(const STR &config_file)
{
	STR					content;
	ConfigurationFile	file(config_file);

	if (file.getTypePath(file.getPath()) != 1)
		throw ErrorException("FIle isn't valid");
	if (file.checkConfigFile(file.getPath(), 4) == -1)
		throw ErrorException("File isn't accesible");
	
	content = file.readFile(config_file);
	if (content.empty())
		throw ErrorException("File is empty");
	removeComments(content);
	removeWhiteSpace(content);
	splitServers(content);
	if(this->_server_config.size() != this->_nb_server)
		throw ErrorException("Config Size Error");
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

void ConfigurationParser::removeComments(STR &content)
{
	size_t	pos;

	pos = content.find('#');
	while (pos != STR::npos)
	{
		size_t pos_end;
		pos_end = content.find('\n', pos);
		content.erase(pos, pos_end - pos);
		pos = content.find('#');
	}
}

void ConfigurationParser::removeWhiteSpace(STR &content)
{
	size_t i = 0;

	while (content[i] && isspace(content[i]))
		i++;
	content = content.substr(i);
	i = content.length() - 1;
	while (i > 0 && isspace(content[i]))
		i--;
	content = content.substr(0, i + 1);
}

void ConfigurationParser::splitServers(STR &content)
{
	size_t start = 0;
	size_t end = 1;

	if (content.find("server", 0) == std::string::npos)
		throw ErrorException("Server not fount");
	while (start != end && start < content.length())
	{
		start = findServerBegin(start, content);
		end = findServerEnd(start, content);
		if (start == end)
			throw ErrorException("Splitting Servers Error");
		this->_server_config.push_back(content.substr(start, end - start + 1));
		this->_nb_server++;
		start = end + 1;
	}
}

size_t ConfigurationParser::findServerBegin(size_t start, STR &content)
{
	size_t i;

	for (i = start; content[i]; i++)
	{
		if (content[i] == 's')
			break ;
		if (!isspace(content[i]))
			throw ErrorException("Server Content Error: isspace()");
	}
	if (!content[i])
		return (start);
	if (content.compare(i, 6, "server") != 0)
		throw ErrorException("Server Content Error: name");
	i += 6;
	while (content[i] && isspace(content[i]))
		i++;
	if (content[i] == '{')
		return (i);
	else
		throw ErrorException("Server Content Error: '{' found");
}

size_t ConfigurationParser::findServerEnd(size_t start, STR &content)
{
	size_t i;
	size_t j = 0;

	for (i = start + 1; content[i]; i++)
	{
		if (content[i] == '{')
			j++;
		if (content[i] == '}')
		{
			if (!j)
				return (i);
			j--;
		}
	}
	return(start);
}

VECTOR<STR> splitParams(STR line, STR sep)
{
	VECTOR<STR>		str;
	STR::size_type	start, end;

	start = 0;
	end = 0;
	while (1)
	{
		end = line.find_last_not_of(sep, start);
		if(end == STR::npos)
			break;
		STR tmp = line.substr(start, end - start);
		str.push_back(tmp);
		start = line.find_first_not_of(sep, end);
		if (start == STR::npos)
			break;
	}
	return(str);
}

void ConfigurationParser::createServer(STR &config, ServerConfiguration &server)
{
	VECTOR<STR>	params;
	VECTOR<STR>	err_code;
	int			loc = 1;
	bool		autoindex = false;
	bool		max_size = false;

	params = splitParams(config += ' ', STR(" \n\t"));
	if (params.size() < 3)
		throw ErrorException("Failed server validation");
	for (size_t i = 0; i < params.size(); i++)
	{
		if (params[i] == "listen" && (i + 1) < params.size() && loc)
		{
			if (server.getPort())
				throw ErrorException("Dublicated port");
			server.setPort(params[++i]);
		}
		else if (params[i] == "location" && (i + 1) < params.size())
		{
			STR	path;
			i++;
			if(params[i] == "{" || params[i] == "}")
				throw ErrorException("Creating server error: '{}' found");
			path = params[i];
			VECTOR<STR> codes;
			if (params[++i] != "{")
				throw ErrorException("Creating server error: '{' not found");
			i++;
			while(i < params.size() && params[i] != "}")
				codes.push_back(params[++i]);
			server.setLocation(path, codes);
			if(i < params.size() && params[i] != "}")
				throw ErrorException("Creating server error: '}' not found");
			loc = 0;
		}
		else if (params[i] == "host" && (i + 1) < params.size() && loc)
		{
			if(server.getHost())
				throw ErrorException("Duplicated host");
			server.setHost(params[++i]);
		}
		else if (params[i] == "root" && (i + 1) < params.size() && loc)
		{
			if(!server.getRoot().empty())
				throw ErrorException("Duplicated root");
			server.setRoot(params[++i]);
		}
		else if (params[i] == "error_page" && (i + 1) < params.size() && loc)
		{
			while (++i < params.size())
			{
				err_code.push_back(params[i]);
				if(params[i].find(';') != STR::npos)
					break;
				if(i + 1 >= params.size())
					throw ErrorException("Wrong character out of server scope{}");
			}
		}
		else if (params[i] == "client_max_body_size" && (i + 1) < params.size() && loc)
		{
			if (max_size)
				throw ErrorException("Duplicated Client_max_body_size");
			server.setClientMaxBodySize(params[++i]);
			max_size = true;
		}
		else if (params[i] == "server_name" && (i + 1) < params.size() && loc)
		{
			if (!server.getServerName().empty())
				throw  ErrorException("Duplicated Server_name");
			server.setServerName(params[++i]);
		}
		else if (params[i] == "index" && (i + 1) < params.size() && loc)
		{
			if (!server.getIndex().empty())
				throw  ErrorException("Duplicated Index");
			server.setIndex(params[++i]);
		}
		else if (params[i] == "autoindex" && (i + 1) < params.size() && loc)
		{
			if (autoindex)
				throw ErrorException("Duplicated Autoindex of server");
			server.setAutoIndex(params[++i]);
			autoindex = true;
		}
		else if (params[i] != "}" && params[i] != "{")
		{
			if (!loc)
				throw  ErrorException("params after location");
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
	if (ConfigurationFile::checkIsFileReadable(server.getRoot(), server.getIndex()))
		throw ErrorException("Index from config file not found or unreadable");
	if (server.validationLocations())
		throw ErrorException("Locaition is duplicated");
	if (!server.getPort())
		throw ErrorException("Port not found");
	server.setErrorPages(err_code);
	if (!server.checkErrPagesValid())
		throw ErrorException("Incorrect path for error page or number of error");
}

void ConfigurationParser::checkServers()
{
	VECTOR<ServerConfiguration>::iterator it1;
	VECTOR<ServerConfiguration>::iterator it2;

	for (it1 = this->_servers.begin(); it1 != this->_servers.end() - 1; it1++)
	{
		for (it2 = it1 + 1; it2 != this->_servers.end() - 1; it1++)
		{
			if (it1->getPort() == it2->getPort() && it1->getHost() == it2->getHost() && \
				 it1->getServerName() == it2->getServerName())
				throw ErrorException("Server Validation Failed");
		}
	}
}

VECTOR<ServerConfiguration>	ConfigurationParser::getServers(){ return (this->_servers);}

int ConfigurationParser::strCompare(STR str1, STR str2, size_t pos)
{
	size_t	i = 0;

	while (pos < str1.length() && i < str2.length() && str1[pos] == str2[i])
	{
		pos++;
		i++;
	}
	if (i == str2.length() && pos <= str1.length() && (str1.length() == pos || isspace(str1[pos])))
	{
		return(0);
	}
	return(1);
}

int ConfigurationParser::printConfig()
{
	COUT << B_GREEN << "\tConfig :" << DEFAULT << ENDL;
	for (size_t i = 0; i < _servers.size(); i++)
	{
		COUT << "Server # " << i + 1 << ENDL;
		COUT << "Server Name: " << _servers[i].getServerName() << ENDL;
		COUT << "Host: " << _servers[i].getServerName() << ENDL;
		COUT << "Root: " << _servers[i].getRoot() << ENDL;
        COUT << "Index: " << _servers[i].getIndex() << ENDL;
        COUT << "Port: " << _servers[i].getPort() << ENDL;
		COUT << "Max Body Size: " << _servers[i].getClientMaxBodySize() << ENDL;
        COUT << "Error  pages: " << _servers[i].getErrorPages().size() << ENDL;
	}
	return (0);
}
