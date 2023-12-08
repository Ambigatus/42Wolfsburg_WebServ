#include "../include/CGI_Configuration.hpp"

const MAP<STR, STR> &CGIConfig::getEnv() const
{
	return (this->_env);
}

const pid_t &CGIConfig::getCGIpid() const
{
	return (this->_cgi_pid);
}

const STR &CGIConfig::getCGIpath() const
{
	return (this->_cgi_path);
}
