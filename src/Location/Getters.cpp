#include "../../include/Location.hpp"

const STR	&Location::getPath() const
{
	return this->_path;
}

const STR	&Location::getRootLocation() const
{
	return this->_root;
}

const STR	&Location::getIndexLocation() const
{
	return this->_index;
}

const VECTOR<short>	&Location::getMethods() const
{
	return this->_methods;
}

const VECTOR<STR>	&Location::getCGIPath() const
{
	return this->_cgi_path;
}

const VECTOR<STR>	&Location::getCGIExtension() const
{
	return this->_cgi_exten;
}

const bool	&Location::getAutoIndex() const
{
	return this->_autoindex;
}

const STR	&Location::getReturn() const
{
	return this->_return;
}

const STR	&Location::getAlias() const
{
	return this->_alias;
}

const MAP<STR, STR>	&Location::getExtensionPath() const
{
	return this->_exten_path;
}

const size_t	&Location::getMaxBodySize() const
{
	return this->_client_max_body_size;
}

STR	Location::methodsOutput() const
{
	STR output;
	if (_methods[4])
		output.insert(0, "HEAD");
	if (_methods[3])
	{
		if (!output.empty())
			output.insert(0, ", ");
		output.insert(0, "PUT");
	}
	if (_methods[2])
	{
		if (!output.empty())
			output.insert(0, ", ");
		output.insert(0, "DELETE");
	}
	if (_methods[1])
	{
		if (!output.empty())
			output.insert(0, ", ");
		output.insert(0, "POST");
	}
	if (_methods[0])
	{
		if (!output.empty())
			output.insert(0, ", ");
		output.insert(0, "GET");
	}
	return output;
}
