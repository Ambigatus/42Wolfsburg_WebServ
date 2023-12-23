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

void    ServerConfiguration::setLocation(STR path, VECTOR<STR> param)
{
    Location        new_location;
    VECTOR<STR>     methods;
    bool            flag_methods = false;
    bool            flag_autoindex = false;
    bool            flag_max_size = false;
    int             validation;

    new_location.setPath(path);
    for (size_t i = 0; i < param.size(); i++)
    {
        if (param[i] == "root" && (i + 1) < param.size())
        {
            if (!new_location.getRootLocation().empty())
                throw ErrorExeption("ERROR: Root of location is duplicated.");
            validationToken(param[++i]);
            if (ConfigurationFile::getTypePath(param[i]) == 2)
                new_location.setRootLocation(param[i]);
            else
                new_location.setRootLocation(this->_root + param[i]);
        }
        else if ((param[i] == "allow_methods" || param[i] == "methods") && (i + 1) < param.size())
        {
            if (flag_methods)
                throw ErrorExeption("ERROR: Allow methods of location is duplicated.");
            VECTOR<STR> methods;
            while (++i < param.size())
            {
                if (param[i].find(";") != STR::npos)
                {
                    validationToken(param[i]);
                    methods.push_back(param[i]);
                    break;
                }
                else
                {
                    methods.push_back(param[i]);
                    if (i + 1 >= param.size())
                        throw ErrorExeption("ERROR: Token is invalid.");
                }
            }
            new_location.setMethods(methods);
            flag_methods = true;
        }
        else if (param[i] == "autoindex" && (i + 1) < param.size())
        {
            if (path == "/cgi-bin")
                throw ErrorExeption("ERROR: Parameter 'autoindex' is not allowed for CGI.");
            if (flag_autoindex)
                throw ErrorExeption("ERROR: Autoindex of location is duplicated.");
            validationToken(param[++i]);
            new_location.setAutoIndex(param[i]);
            flag_autoindex = true;
        }
        else if (param[i] == "index" && (i + 1) < param.size())
        {
            if (!new_location.getIndexLocation().empty())
                throw ErrorExeption("ERROR: Index of location is duplicated.");
            validationToken(param[++i]);
            new_location.setIndexLocation(param[i]);
        }
        else if (param[i] == "return" && (i + 1) < param.size())
        {
            if (path == "/cgi-bin")
                throw ErrorExeption("ERROR: Parameter 'return' is not allowed for CGI.");
            if (!new_location.getReturn().empty())
                throw ErrorExeption("ERROR: Return of location is duplicated.");
            validationToken(param[++i]);
            new_location.setReturn(param[i]);
        }
        else if (param[i] == "alias" && (i + 1) < param.size())
        {
            if (path == "/cgi-bin")
                throw ErrorExeption("ERROR: Parameter 'alias' is not allowed for CGI.");
            if(!new_location.getAlias().empty())
                throw ErrorExeption("ERROR: Alias of location is duplicated");
            validationToken(param[++i]);
            new_location.setAlias(param[i]);
        }
        else if (param[i] == "cgi_ext" && (i + 1) < param.size())
        {
            VECTOR<STR> extension;
            while (++i < param.size())
            {
                if (param[i].find(";") != STR::npos)
                {
                    validationToken(param[i]);
                    extension.push_back(param[i]);
                    break;
                }
                else
                {
                    extension.push_back(param[i]);
                    if (i + 1 >= param.size())
                        throw ErrorExeption("ERROR: Token is invalid.");
                }
            }
            new_location.setCGIExtension(extension);
        }
        else if (param[i] == "cgi_path" && (i + 1) < param.size())
        {
            VECTOR<STR> path;
            while (++i < param.size())
            {
                if (param[i].find(";") != STR::npos)
                {
                    validationToken(param[i]);
                    path.push_back(param[i]);
                    break;
                }
                else
                {
                    path.push_back(param[i]);
                    if (i + 1 >= param.size())
                        throw ErrorExeption("ERROR: Token is invalid.");
                }
                if (param[i].find("/python") == STR::npos && param[i].find("/bash") == STR::npos)
                    throw ErrorExeption("ERROR: CGI path is invalid.");
            }
            new_location.setCGIPath(path);
        }
        else if (param[i] == "client_max_body_size" && (i + 1) < param.size())
        {
            if (flag_max_size)
                throw ErrorExeption("ERROR: Max body size is duplicated.");
            validationToken(param[++i]);
            new_location.setMaxBodySize(param[i]);
            flag_max_size = true;
        }
        else if (i < param.size())
            throw ErrorExeption("ERROR: Parameter in the location is invalid.");
    }
    if (new_location.getPath() != "/cgi-bin" && new_location.getIndexLocation().empty())
        new_location.setIndexLocation(this->_index);
    if (!flag_max_size)
        new_location.setMaxBodySize(this->_client_max_body_size);
    validation = checkLocationValid(new_location);
    if (validation == 1)
        throw ErrorExeption("ERROR: Failed CGI validation.");
    else if (validation == 2)
        throw ErrorExeption("ERROR: Failed path in location validation.");
    else if (validation == 3)
        throw ErrorExeption("ERROR: Failed redirection file in location validation.");
    else if (validation == 4)
        throw ErrorExeption("ERROR: Failed alias file in location validation.");
    this->_locations.push_back(new_location);
}

void    ServerConfiguration::setFD(int fd)
{
    this->_listen_fd = fd;
}
