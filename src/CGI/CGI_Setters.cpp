#include "../../include/CGI_Configuration.hpp"

void	CGIConfig::setCGIpid(pid_t cgi_pid)
{
	this->_cgi_pid = cgi_pid;
}

void	CGIConfig::setCGIpath(const std::string &cgi_path)
{
	this->_cgi_path = cgi_path;
}
