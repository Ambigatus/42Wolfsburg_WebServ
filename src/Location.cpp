#include "../include/Location.hpp"

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

Location::Location(const Location &copy)
{
	this->_path = copy._path;
	this->_root = copy._root;
	this->_autoindex = copy._autoindex;
	this->_index = copy._index;
	this->_cgi_path = copy._cgi_path;
	this->_cgi_exten = copy._cgi_exten;
	this->_return = copy._return;
	this->_alias = copy._alias;
	this->_methods = copy._methods;
	this->_exten_path = copy._exten_path;
	this->_client_max_body_size = copy._client_max_body_size;
}

Location &Location::operator=(const Location &copy)
{
	if (this != &copy)
	{
		this->_path = copy._path;
		this->_root = copy._root;
		this->_autoindex = copy._autoindex;
		this->_index = copy._index;
		this->_cgi_path = copy._cgi_path;
		this->_cgi_exten = copy._cgi_exten;
		this->_return = copy._return;
		this->_alias = copy._alias;
		this->_methods = copy._methods;
		this->_exten_path = copy._exten_path;
		this->_client_max_body_size = copy._client_max_body_size;
	}
	return *this;
}

Location::~Location() {}

