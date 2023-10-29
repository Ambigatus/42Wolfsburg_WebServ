#include "../../include/CGI_Configuration.hpp"

void	CGIConfig::setSGIpid(pid_t cgi_pid)
{
	this->_cgi_pid = cgi_pid;
}

void	CGIConfig::setSGIpath(const STR &cgi_path)
{
	this->_cgi_path = cgi_path;
}

