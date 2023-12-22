#include "../include/Response.hpp"


Response::Response()
{
	_target_file = "";
	_body.clear();
	_body_len = 0;
	_response_ready = "";
	_location = "";
	_code = 0;
	_cgi = 0;
	_cgi_response_len = 0;
	_auto_index = 0;
}

Response::Response(Request &req) : _request(req)
{
	_target_file = "";
	_body.clear();
	_body_len = 0;
	_response_ready = "";
	_location = "";
	_code = 0;
	_cgi = 0;
	_cgi_response_len = 0;
	_auto_index = 0;
}

Response::~Response(){}

Response::Response(const Response &copy)
{
	if (this != &copy)
	{
		this->_target_file = copy._target_file;
		this->_body_len = copy._body_len;
		this->_response_ready = copy._response_ready;
		this->_location = copy._location;
		this->_code = copy._code;
		this->_cgi = copy._cgi;
		this->_cgi_response_len = copy._cgi_response_len;
		this->_auto_index = copy._auto_index;
	}
	return ;
}


Response &Response::operator=(const Response &copy)
{
	if (this != &copy)
	{
		this->_target_file = copy._target_file;
		this->_body_len = copy._body_len;
		this->_response_ready = copy._response_ready;
		this->_location = copy._location;
		this->_code = copy._code;
		this->_cgi = copy._cgi;
		this->_cgi_response_len = copy._cgi_response_len;
		this->_auto_index = copy._auto_index;
	}
	return *this;
}

//const std::unordered_map<STR, STR>	Response::mimeTypes = {
//	{".html", "text/html"},
//	{".htm", "text/html"},
//	{".css", "text/css"},
//	{".ico", "image/x-icon"},
//	{".avi", "video/x-msvideo"},
//	{".bmp", "image/bmp"},
//	{".doc", "application/msword"},
//	{".gif", "image/gif"},
//	{".gz", "application/x-gzip"},
//	{".ico", "image/x-icon"},
//	{".jpg", "image/jpeg"},
//	{".jpeg", "image/jpeg"},
//	{".png", "image/png"},
//	{".txt", "text/plain"},
//	{".mp3", "audio/mp3"},
//	{".pdf", "application/pdf"},
//	{"default", "text/html"}
//};


//void	Response::addMimeType()
//{
//	_response_ready.append("Content-Type: ");
//
//	size_t dotPos = _target_file.rfind(".");
//	if (dotPos != STR::npos && _code == 200)
//	{
//		STR extension = _target_file.substr(dotPos);
//		std::unordered_map<STR, STR>::const_iterator it = mimeTypes.find(extension);
//		if (it != mimeTypes.end())
//			_response_ready.append(it->second);
//		else
//			_response_ready.append(mimeTypes.at(".default"));
//
//	}
//	else
//	{
//		_response_ready.append(mimeTypes.at(".default"));
//	}
//	_response_ready.append("\r\n");
//}

void Response::addMimeType()
{
    _response_ready.append("Content-Type: ");

    size_t dotPos = _target_file.rfind(".");
    if (dotPos != STR::npos && _code == 200)
    {
        STR extension = _target_file.substr(dotPos);
        STR mimeType = _mime.getFileType(extension);
        _response_ready.append(mimeType);
    }
    else
    {
        _response_ready.append(_mime.getFileType(".default"));
    }

    _response_ready.append("\r\n");
}

void	Response::addContentLen()
{
	std::stringstream	ss;
	ss << _response_body.length();
	_response_ready.append("Content-Length: " + ss.str() + "\r\n");
}

void	Response::addConnection()
{
	if(_request.getHeader("connection") == "keep-alive")
		_response_ready.append("Connection: keep-alive\r\n");
}

void	Response::addServer()
{
	_response_ready.append("Server: AMAnix\r\n");
}

void	Response::addLocation()
{
	if (_location.length())
		_response_ready.append("Location: " + _location + "\r\n");
}

void	Response::addDate()
{
	char	date[1000];
	time_t	now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", &tm);
	_response_ready.append("Date: ");
	_response_ready.append(date);
	_response_ready.append("\r\n");
}

void	Response::setHeaders()
{
	addMimeType();
	addContentLen();
	addConnection();
	addServer();
	addLocation();
	addDate();
	_response_ready.append("\r\n");
}

static bool	fileExists(const STR& file)
{
	std::ifstream f(file.c_str());
	return (f.good());
}

static bool isDirectory(STR path)
{
	struct stat	file_stat;
	if (stat(path.c_str(), &file_stat) != 0)
		return(false);
	return (S_ISDIR(file_stat.st_mode));
}

static bool	isAllowedMethod(Method &method, Location &location, short &code)
{
	VECTOR<short> methods = location.getMethods();
	if ((method == GET && !methods[0]) ||
		(method == POST && !methods[1]) ||
		(method == DELETE && !methods[2]) ||
		(method == PUT && !methods[3]) ||
		(method == HEAD && !methods[4]))
	{
		code = 405;
		return (1);
	}
	return (0);
}

static bool	checkReturn(Location &location, short &code, STR &loc)
{
	if (!location.getReturn().empty())
	{
		code = 301;
		loc = location.getReturn();
		if (loc[0] != '/')
			loc.insert(loc.begin(), '/');
		return 1;
	}
	return 0;
}

static STR	makePath(STR path1, STR path2, STR path3)
{
	STR	path;
	int len1 = path1.length();
	int	len2 = path2.length();

	if (path1[len1 - 1] == '/' && (!path2.empty() && path2[0] == '/'))
		path2.erase(0, 1);
	if (path1[len1 - 1] != '/' && (!path2.empty() && path2[0] == '/'))
		path1.insert(path1.end(), '/');
	if (path2[len2 - 1] == '/' && (!path3.empty() && path3[0] == '/'))
		path3.erase(0, 1);
	if (path2[len2 - 1] != '/' && (!path3.empty() && path3[0] == '/'))
		path2.insert(path1.end(), '/');
	path = path1 + path2 + path3;
	return(path);
}

static void	replaceAlias(Location &location, Request &request, STR &target_file)
{
	target_file = makePath(location.getAlias(), request.getPath().substr(location.getPath().length()), "");
}

static void	appendRoot(Location &location, Request &request, STR &target_file)
{
	target_file = makePath(location.getRootLocation(), request.getPath(), "");
}

int	Response::handleCgiTemp(STR &loc_key)
{
	STR path = _target_file;
	_cgi_conf.cleaning();
	_cgi_conf.setCGIpath(path);
	_cgi = 1;
	if (pipe(_cgi_fd) < 0)
	{
		_code = 500;
		return 1;
	}
	_cgi_conf.CGIEnvInitialization(_request, _server.getLocationKey(loc_key));
	_cgi_conf.execute(this->_code);
	return 0;
}

int	Response::handleCgi(STR &loc_key)
{
	STR		path;
	STR		extention;
	size_t	pos;

	path = this->_request.getPath();
	if (path[0] && path[0] == '/')
		path.erase(0, 1);
	if (path == "cgi-bin")
		path += "/" + _server.getLocationKey(loc_key)->getIndexLocation();
	else if (path == "cgi-bin/")
		path.append(_server.getLocationKey(loc_key)->getIndexLocation());

	pos = path.find(".");
	if (pos == STR::npos)
	{
		_code = 501;
		return 1;
	}

	extention = path.substr(pos);
	if (extention != ".py" && extention != ".sh")
	{
		_code = 501;
		return 1;
	}

	if (ConfigurationFile::getTypePath(path) != 1)
	{
		_code = 404;
		return 1;
	}

	if (ConfigurationFile::checkConfigFile(path, 1) == -1 || ConfigurationFile::checkConfigFile(path, 3) == -1)
	{
		_code = 403;
		return 1;
	}

	if (isAllowedMethod(_request.getMethod(), *_server.getLocationKey(loc_key), _code))
		return 1;
	
	_cgi_conf.cleaning();
	_cgi_conf.setCGIpath(path);
	_cgi = 1;
	if (pipe(_cgi_fd) < 0)
	{
		_code = 500;
		return 1;
	}
	_cgi_conf.envInitializatation(_request, _server.getLocationKey(loc_key));
	_cgi_conf.execute(this->_code);
	return 0;
}

static void getLocationMatch(STR &path, VECTOR<Location> locations, STR &location_key)
{
	size_t best_match = 0;

	for (VECTOR<Location>::const_iterator it = locations.begin(); it != locations.end(); ++it)
	{
		if (path.find(it->getPath()) == 0)
		{
			if (it->getPath() == "/" || path.length() || path[it->getPath().length()] == '/')
			{
				if(it->getPath().length() > best_match)
				{
					best_match = it->getPath().length();
					location_key = it->getPath();
				}
			}
		}
	}
}

int Response::handleTarget()
{
	STR location_key;
	getLocationMatch(_request.getPath(), _server.getLocations(), location_key);
	if (location_key.length() > 0)
	{
		Location target_location = *_server.getLocationKey(location_key);
		
		if (isAllowedMethod(_request.getMethod(), target_location, _code))
		{
			COUT << "METHOD NOT ALLOWED \n";
			return 1;
		}
		if (_request.getBody().length() > target_location.getMaxBodySize())
		{
			_code = 413;
			return 1;
		}
		if (checkReturn(target_location, _code, _location))
			return 1;
		
		if (target_location.getPath().find("cgi-bin") != STR::npos)
			return(handleCgi(location_key));

		if (!target_location.getAlias().empty())
			replaceAlias(target_location, _request, _target_file);
		else
			appendRoot(target_location, _request, _target_file);

		if (!target_location.getCGIExtension().empty())
		{
			if(_target_file.rfind(target_location.getCGIExtension()[0]) != STR::npos)
				return(handleCgiTemp(location_key));
		}

		if (isDirectory(_target_file))
		{
			if (_target_file[_target_file.length() - 1] != '/')
			{
				_code = 301;
				_location = _request.getPath() + "/";
				return 1;
			}
			if (!target_location.getIndexLocation().empty())
				_target_file += target_location.getIndexLocation();
			else
				_target_file += _server.getIndex();

			if(!fileExists(_target_file))
			{
				if (target_location.getAutoIndex())
				{
					_target_file.erase(_target_file.find_last_of('/') + 1);
					_auto_index = true;
					return 0;
				}
				else
				{
					_code = 403;
					return 1;
				}
			}

			if (isDirectory(_target_file))
			{
				_code = 301;
				if(!target_location.getIndexLocation().empty())
					_location = makePath(_request.getPath(), target_location.getIndexLocation(), "");
				else
					_location = makePath(_request.getPath(), _server.getIndex(), "");
				if (_location[_location.length() - 1] != '/')
					_location.insert(_location.end(), '/');

				return 1;
			}
		}
	}
	else
	{
		_target_file = makePath(_server.getRoot(), _request.getPath(), "");
		if (isDirectory(_target_file))
		{
			if(_target_file[_target_file.length() -1] != '/')
			{
				_code = 301;
				_location = _request.getPath() + "/";
				return (1);
			}
			_target_file += _server.getIndex();
			if (!fileExists(_target_file))
			{
				_code = 403;
				return (1);
			}
			if (isDirectory(_target_file))
			{
				_code = 301;
				_location = makePath(_request.getPath(), _server.getIndex(), "");
				if(_location[_location.length() - 1] != '/')
					_location.insert(_location.end(), '/');
				return 1;
			}
		}
	}
	return 0;
}

bool Response::reqError()
{
	if(_request.errorCode())
	{
		_code = _request.errorCode();
		return 1;
	}
	return 0;
}

void Response::setServerErrorPages()
{
	_response_body = showErrorPage(_code);
}

void Response::buildErrorBody()
{
	if (!_server.getErrorPages().count(_code) || _server.getErrorPages().at(_code).empty() ||
	_request.getMethod() == DELETE || _request.getMethod() == POST)
	{
		setServerErrorPages();
	}
	else
	{
		if (_code >= 400 && _code < 500)
		{
			_location = _server.getErrorPages().at(_code);
			if(_location[0] != '/')
				_location.insert(_location.begin(), '/');
			_code = 302;
		}

		_target_file = _server.getRoot() + _server.getErrorPages().at(_code);
		short old_code = _code;
		if(readFile())
		{
			_code = old_code;
			_response_body = showErrorPage(_code);
		}
	}
}

void Response::buildResponse()
{
	if (reqError() || buildBody())
		buildErrorBody();
	if(_cgi)
		return ;
	else if (_auto_index)
	{
		COUT << "AUTO index " << ENDL;
		if(createHTMLIndex(_target_file, _body, _body_len))
		{
			_code = 500;
			buildErrorBody();
		}
		else
			_code = 200;
		_response_ready.insert(_response_body.begin(), _body.begin(), _body.end());
	}
	setStatusLine();
	setHeaders();
	if (_request.getMethod() != HEAD && (_request.getMethod() == GET || _code != 200))
		_response_ready.append(_response_body);
}

void Response::setErrorResponse(short code)
{
	_response_ready = "";
	_code = code;
	_response_body = "";
	buildErrorBody();
	setStatusLine();
	setHeaders();
	_response_ready.append(_response_body);
}

STR Response::getResponse()
{
	return(_response_ready);
}

size_t Response::getLen() const 
{
	return (_response_ready.length());
}

void Response::setStatusLine()
{
	_response_ready.append("HTTP/1.1 " + toString(_code) + " ");
	_response_ready.append(statusCodeList(_code));
	_response_ready.append("\r\n");
}

int	Response::buildBody()
{
	if (_request.getBody().length() > _server.getClientMaxBodySize())
	{
		_code = 413;
		return(1);
	}
	if (handleTarget())
		return 1;
	if (_cgi || _auto_index)
		return 0;
	if (_code)
		return 0;
	if (_request.getMethod() == GET || _request.getMethod() == HEAD)
	{
		if (readFile())
			return 1;
	}
	else if (_request.getMethod() == POST || _request.getMethod() == PUT)
	{
		if (fileExists(_target_file) && _request.getMethod() == POST)
		{
			_code = 204;
			return 0;
		}
		std::ofstream file(_target_file.c_str(), std::ios::binary);
		if (file.fail())
		{
			_code = 404;
			return 1;
		}

		if (_request.getFlag())
		{
			STR body = _request.getBody();
			body = removeBoundary(body, _request.getBoundary());
			file.write(body.c_str(), body.length());
		}
		else
		{
			file.write(_request.getBody().c_str(), _request.getBody().length());
		}
	}
	else if (_request.getMethod() == DELETE)
	{
		if (!fileExists(_target_file))
		{
			_code = 404;
			return 1;
		}
		if (remove(_target_file.c_str()) != 0)
		{
			_code = 500;
			return 1;
		}
	}
	_code = 200;
	return 0;
}

int Response::readFile()
{
	std::ifstream file(_target_file.c_str());

	if (file.fail())
	{
		_code = 404;
		return (1);
	}

	std::ostringstream ss;
		ss << file.rdbuf();
	_response_body = ss.str();
	return 0;
}

void Response::setServer(ServerConfiguration &server)
{
	_server = server;
}

void Response::setRequest(Request &request)
{
	_request = request;
}

void Response::cutResponse(size_t i)
{
	_response_ready = _response_ready.substr(i);
}

void Response::clear()
{
	_target_file.clear();
	_body.clear();
	_body_len = 0;
	_response_ready.clear();
	_location.clear();
	_code = 0;
	_cgi = 0;
	_cgi_response_len = 0;
	_auto_index = 0;
}

int Response::getCode() const
{
	return (_code);
}

int Response::getCgiState()
{
	return (_cgi);
}

STR Response::removeBoundary(STR &body, STR &boundary)
{
	STR buffer;
	STR new_body;
	STR filename;
	bool is_boundary = false;
	bool is_content = false;

	if (body.find("--" + boundary) != STR::npos && body.find("--" + boundary + "--") != STR::npos)
	{
		for (size_t i = 0; i < body.size(); i++)
		{
			buffer.clear();
			while(body[i] != '\n')
			{
				buffer += body[i];
				i++;
			}
			if (!buffer.compare(("--" + boundary + "--\r")))
			{
				is_content = true;
				is_boundary = false;
			}
			if (!buffer.compare(("--" + boundary + "\r")))
			{
				is_boundary = true;
			}
			if(is_boundary)
			{
				if (!buffer.compare(0, 31, "Content-Disposition: form-data;"))
				{
					size_t start = buffer.find("filename=\"");
					if (start != STR::npos)
					{	
						size_t end = buffer.find("\"", start + 10);
						if (end != STR::npos)
							filename = buffer.substr(start + 10, end);
					}
				}
				else if (!buffer.compare(0, 1, "\r") && !filename.empty())
				{
					is_boundary = false;
					is_content = true;
				}
			}
			else if (is_content)
			{
				if (!buffer.compare(("--" + boundary + "\r")))
					is_boundary = true;
				else if (!buffer.compare(("--" + boundary + "--\r")))
				{
					new_body.erase(new_body.end() - 1);
					break ;
				}
				else
					new_body += (buffer + "\n");
			}
		}
	}

	body.clear();
	return (new_body);
}

void Response::setCgiState(int state)
{
	_cgi = state;
}
