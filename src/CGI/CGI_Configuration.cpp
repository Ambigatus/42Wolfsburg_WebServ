#include "../../include/CGI_Configuration.hpp"

CGIConfig::CGIConfig()
{
	this->_cgi_pid = -1;
	this->_exit_status = 0;
	this->_cgi_path = "";
	this->_char_env = NULL;
	this->_argv = NULL;
}

CGIConfig::CGIConfig(STR path)
{
	this->_cgi_pid = -1;
	this->_exit_status = 0;
	this->_cgi_path = path;
	this->_char_env = NULL;
	this->_argv = NULL;
}

CGIConfig::~CGIConfig()
{
	if (this->_char_env)
	{
		for (int i = 0; this->_char_env[i]; i++)
			free(this->_char_env[i]);
		free(this->_char_env);
	}
	if (this->_argv)
	{
		for (int i = 0; this->_argv[i]; i++)
			free(_argv[i]);
		free(_argv);
	}
	this->_env.clear();
}

CGIConfig::CGIConfig(const CGIConfig &copy)
{
	this->_env = copy._env;
	this->_char_env = copy._char_env;
	this->_argv = copy._argv;
	this->_cgi_path = copy._cgi_path;
	this->_cgi_pid = copy._cgi_pid;
	this->_exit_status = copy._exit_status;
}

CGIConfig &CGIConfig::operator=(const CGIConfig &copy)
{
	if (this != &copy)
	{
		this->_env = copy._env;
		this->_char_env = copy._char_env;
		this->_argv = copy._argv;
		this->_cgi_path = copy._cgi_path;
		this->_cgi_pid = copy._cgi_pid;
		this->_exit_status = copy._exit_status;
	}
	return (*this);
}

void	CGIConfig::envInitializatation(HTTPRequest &request, const VECTOR<Location>::iterator iter_loc)
{
	STR		cgi_exec = ("cgi-bin/" + iter_loc->getCGIpath()[0]).c_str();
	char	*cwd = getcwd(NULL, 0);
	if (_cgi_path[0] != '/')
	{
		STR tmp(cwd);
		tmp.append("/");
		if (_cgi_path.length() > 0)
			_cgi_path.insert(0, tmp);
	}
	// if (request.getMethod() == POST) i need here part from HTTP request part
	{

	}
}