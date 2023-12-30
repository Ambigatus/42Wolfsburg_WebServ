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

void	CGIConfig::CGIEnvInitialization(Request &request, const VECTOR<Location>::iterator iter_loc)
{
	STR		cgi_exec = ("cgi-bin/" + iter_loc->getCGIPath()[0]).c_str();
	char	*cwd = getcwd(NULL, 0);
	if (_cgi_path[0] != '/')
	{
		STR tmp(cwd);
		tmp.append("/");
		if (_cgi_path.length() > 0)
			_cgi_path.insert(0, tmp);
	}
	if (request.getMethod() == POST)
	{
		std::stringstream out;
		out << request.getBody().length();
		this->_env["CONTENT_LENGTH"] = out.str();
		this->_env["CONTENT_TYPE"] = request.getHeader("content-type");
	}

	this->_env["GATEWAY_INTERFACE"] = STR("CGI/1.1");
	this->_env["SCRIPT_NAME"] = cgi_exec;
	this->_env["SCRIPT_FILENAME"] = this->_cgi_path;
	this->_env["PATH_INFO"] = this->_cgi_path;
	this->_env["PATH_TRANSLATED"] = this->_cgi_path;
	this->_env["REQUEST_URI"] = this->_cgi_path;
	this->_env["SERVER_NAME"] = request.getHeader("host");
	this->_env["SERVER_PORT"] = "8002";
	this->_env["REQUEST_METHOD"] = request.getMethodStr();
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["REDIRECT_STATUS"] = "200";
	this->_env["SERVER_SOFTWARE"] = "AMANIX";

	MAP<STR, STR> request_headers = request.getHeaders();
	for (MAP<STR, STR>::iterator iter = request_headers.begin();
		iter != request_headers.end(); ++iter)
	{
		STR name = iter->first;
		std::transform(name.begin(), name.end(), name.begin(), ::toupper);
		STR key = "HTTP_" + name;
		_env[key] = iter->second;
	}
	this->_char_env = (char **)calloc(sizeof(char *), this->_env.size() + 1);
	MAP<STR, STR>::const_iterator iter = this->_env.begin();
	for (int i = 0; iter != this->_env.end(); iter++, i++)
	{
		STR temp = iter->first + "=" + iter->second;
		this->_char_env[i] = strdup(temp.c_str());
	}
	this->_argv = (char **)malloc(sizeof(char *) * 3);
	this->_argv[0] = strdup(cgi_exec.c_str());
	this->_argv[1] = strdup(this->_cgi_path.c_str());
	this->_argv[2] = NULL;
}

void	CGIConfig::envInitializatation(Request &request, const VECTOR<Location>::iterator iter_loc)
{
	int			position;
	STR			extension;
	STR			extension_path;

	extension = this->_cgi_path.substr(this->_cgi_path.find("."));
    MAP<STR, STR>::iterator iter_path = iter_loc->_exten_path.find(extension);
    if (iter_path == iter_loc->_exten_path.end())
        return ;
    extension_path = iter_loc->_exten_path[extension];

    this->_env["AUTH_TYPE"] = "Basic";
    this->_env["CONTENT_LENGTH"] = request.getHeader("content-length");
    this->_env["CONTENT_TYPE"] = request.getHeader("content-type");
    this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
    position = startFinder(this->_cgi_path, "cgi-bin/");
    this->_env["SCRIPT_NAME"] = this->_cgi_path;
    this->_env["SCRIPT_FILENAME"] = ((position < 0 || (size_t)(position + 8) > this->_cgi_path.size()) ? "" : this->_cgi_path.substr(position + 8, this->_cgi_path.size()));
    this->_env["PATH_INFO"] = getPathInfo(request.getPath(), iter_loc->getCGIExtension());
    this->_env["PATH_TRANSLATED"] = iter_loc->getRootLocation() + (this->_env["PATH_INFO"] == "" ? "/" : this->_env["PATH_INFO"]);
    this->_env["QUERY_STRING"] = decode(request.getQuery());
    this->_env["REMOTE_ADDRESS"] = request.getHeader("host");
    position = startFinder(request.getHeader("host"), ":");
    this->_env["SERVER_NAME"] = (position > 0 ? request.getHeader("host").substr(0, position) : "");
    this->_env["SERVER_PORT"] = (position > 0 ? request.getHeader("host").substr(position + 1, request.getHeader("host").size()) : "");
    this->_env["REQUEST_METHOD"] = request.getMethodStr();
    this->_env["HTTP_COOKIE"] = request.getHeader("cookie");
    this->_env["DOCUMENT_ROOT"] = iter_loc->getRootLocation();
    this->_env["REQUEST_URI"] = request.getPath() + request.getQuery();
    this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
    this->_env["REDIRECT_STATUS"] = "200";
    this->_env["SERVER_SOFTWARE"] = "AMANIX";

    this->_char_env = (char**)calloc(sizeof(char *), this->_env.size() + 1);
    MAP<STR, STR>::const_iterator iter = this->_env.begin();
    for (int i = 0; iter != this->_env.end(); iter++, i++)
    {
        STR temp = iter->first + "=" + iter->second;
        this->_char_env[i] = strdup(temp.c_str());
    }
    this->_argv = (char**)malloc(sizeof(char *) * 3);
    this->_argv[0] = strdup(extension_path.c_str());
    this->_argv[1] = strdup(this->_cgi_path.c_str());
    this->_argv[2] = NULL;
}

void	CGIConfig::execute(short &error_code)
{
	if (this->_argv[0] == NULL || this->_argv[1] == NULL)
	{
		error_code = 500;
		return ;
	}
	if (pipe(pipe_in) < 0)
	{
		Logger::messageLog(B_RED, CONSOLE_OUTPUT, "pipe() error. Failed.");
		close(pipe_in[0]);
		close(pipe_in[1]);
		error_code = 500;
		return ;
	}
	this->_cgi_pid = fork();
	if (this->_cgi_pid == 0)
	{
		dup2(pipe_in[0], STDIN_FILENO);
		dup2(pipe_out[1], STDIN_FILENO);
		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_out[1]);
		this->_exit_status = execve(this->_argv[0], this->_argv, this->_char_env);
		exit(this->_exit_status);
	}
	else if (this->_cgi_pid > 0){}
	else
	{
		COUT << B_RED << "Error: fork failed." << DEFAULT << ENDL;
		error_code = 500;
	}
}

int	CGIConfig::startFinder(const STR path, const STR delim)
{
	if (path.empty())
		return -1;
	size_t  temp = path.find(delim);
	if (temp != STR::npos)
		return (temp);
	else
		return -1;
}

STR	CGIConfig::decode(STR &path)
{
	size_t	token = path.find("%");
	while (token != STR::npos)
	{
		if (path.length() < token + 2)
			break;
		char decimal = hexToDec(path.substr(token + 1, 2));
		path.replace(token, 3, toString(decimal));
		token = path.find("%");
	}
	return (path);
}

STR	CGIConfig::getPathInfo(STR &path, VECTOR<STR> ext)
{
	STR		temp;
	size_t	start;
	size_t	end;

	for (VECTOR<STR>::iterator iter_ext = ext.begin(); iter_ext != ext.end(); iter_ext++)
	{
		start = path.find(*iter_ext);
		if (start != STR::npos)
			break;
	}
	if (start == STR::npos)
		return "";
	if (start + 3 >= path.size())
		return "";
	temp = path.substr(start + 3, path.size());
	if (!temp[0] || temp[0] != '/')
		return "";
	end = temp.find("?");
	return (end == STR::npos ? temp : temp.substr(0, end));
}

void	CGIConfig::cleaning()
{
	this->_cgi_pid = -1;
	this->_exit_status = 0;
	this->_cgi_path = "";
	this->_char_env = NULL;
	this->_argv = NULL;
	this->_env.clear();
}
