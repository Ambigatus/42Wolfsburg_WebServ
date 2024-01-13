#include "../inc/Location.hpp"

Location::Location()
{
	this->_path = "";
	this->_root = "";
	this->_autoindex = false;
	this->_index = "";
	this->_return = "";
	this->_alias = "";
	this->_client_max_body_size = MAX_CONTENT_LENGTH;
	this->_methods.reserve(5);
	this->_methods.push_back(1);
	this->_methods.push_back(0);
	this->_methods.push_back(0);
	this->_methods.push_back(0);
	this->_methods.push_back(0);
}

Location::Location(const Location &other)
{
	this->_path = other._path;
	this->_root = other._root;
	this->_autoindex = other._autoindex;
	this->_index = other._index;
	this->_cgi_path = other._cgi_path;
	this->_cgi_ext = other._cgi_ext;
	this->_return = other._return;
	this->_alias = other._alias;
    this->_methods = other._methods;
	this->_ext_path = other._ext_path;
	this->_client_max_body_size = other._client_max_body_size;
}

Location &Location::operator=(const Location &rhs)
{
	if (this != &rhs)
	{
		this->_path = rhs._path;
		this->_root = rhs._root;
		this->_autoindex = rhs._autoindex;
		this->_index = rhs._index;
		this->_cgi_path = rhs._cgi_path;
		this->_cgi_ext = rhs._cgi_ext;
		this->_return = rhs._return;
		this->_alias = rhs._alias;
		this->_methods = rhs._methods;
		this->_ext_path = rhs._ext_path;
		this->_client_max_body_size = rhs._client_max_body_size;
    }
	return (*this);
}

Location::~Location() { }

void Location::setPath(STR parametr)
{
	this->_path = parametr;
}

void Location::setRootLocation(STR parametr)
{
	if (ConfigurationFile::getTypePath(parametr) != 2)
		throw ServerConfiguration::ErrorException("root of location");
	this->_root = parametr;
}

void Location::setMethods(std::vector<STR> methods)
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
			throw ServerConfiguration::ErrorException("Allow method not supported " + methods[i]);
	}
}

void Location::setAutoindex(STR parametr)
{
	if (parametr == "on" || parametr == "off")
		this->_autoindex = (parametr == "on");
	else
		throw ServerConfiguration::ErrorException("Wrong autoindex");
}

void Location::setIndexLocation(STR parametr)
{
	this->_index = parametr;
}

void Location::setReturn(STR param)
{
	this->_return = param;
}

void Location::setAlias(STR parametr)
{
	this->_alias = parametr;
}

void Location::setCgiPath(std::vector<STR> path)
{
	this->_cgi_path = path;
}

void Location::setCgiExtension(std::vector<STR> extension)
{
	this->_cgi_ext = extension;
}

void Location::setMaxBodySize(STR param)
{
	unsigned long body_size = 0;

	for (size_t i = 0; i < param.length(); i++)
	{
		if (param[i] < '0' || param[i] > '9')
			throw ServerConfiguration::ErrorException("Wrong syntax: client_max_body_size");
	}
	if (!strToInt(param))
		throw ServerConfiguration::ErrorException("Wrong syntax: client_max_body_size");
	body_size = strToInt(param);
	this->_client_max_body_size = body_size;
}

void Location::setMaxBodySize(unsigned long param)
{
	this->_client_max_body_size = param;
}

const STR &Location::getPath() const
{
	return (this->_path);
}

const STR &Location::getRootLocation() const
{
	return (this->_root);
}

const STR &Location::getIndexLocation() const
{
	return (this->_index);
}

const std::vector<short> &Location::getMethods() const
{
	return (this->_methods);
}

const std::vector<STR> &Location::getCgiPath() const
{
	return (this->_cgi_path);
}

const std::vector<STR> &Location::getCgiExtension() const
{
	return (this->_cgi_ext);
}

const bool &Location::getAutoindex() const
{
	return (this->_autoindex);
}

const STR &Location::getReturn() const
{
	return (this->_return);
}

const STR &Location::getAlias() const
{
	return (this->_alias);
}

const std::map<STR, STR> &Location::getExtensionPath() const
{
	return (this->_ext_path);
}

const unsigned long &Location::getMaxBodySize() const
{
	return (this->_client_max_body_size);
}

STR Location::getPrintMethods() const
{
	STR res;
	if (_methods[4])
		res.insert(0, "HEAD");
	if (_methods[3])
	{
		if (!res.empty())
			res.insert(0, ", ");
		res.insert(0, "PUT");
	}
	if (_methods[2])
	{
		if (!res.empty())
			res.insert(0, ", ");
		res.insert(0, "DELETE");
	}
	if (_methods[1])
	{
		if (!res.empty())
			res.insert(0, ", ");
		res.insert(0, "POST");
	}
	if (_methods[0])
	{
		if (!res.empty())
			res.insert(0, ", ");
		res.insert(0, "GET");
	}
	return (res);
}