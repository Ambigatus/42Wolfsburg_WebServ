#include "../include/Request.hpp"

Request::Request()
	: _method(NONE),
	  _state(Request_Line),
	  _max_body_size(0),
	  _body_len(0),
	  _err_code(0),
	  _chunk_len(0),
	  _method_index(1),
	  _ver_major(0),
	  _ver_minor(0),
	  _fields_done(false),
	  _fbody(false),
	  _fbody_done(false),
	  _fcompete(false),
	  _fchunked(false),
	  _flag(false)
{
	_method_str[GET] = "GET";
	_method_str[POST] = "POST";
	_method_str[DELETE] = "DELETE";
	_method_str[PUT] = "PUT";
	_method_str[HEAD] = "HEAD";
	_path = "";
	_query = "";
	_fragment = "";
	_storage = "";
	_key_storage = "";
	_body_str = "";
}

Request::~Request() {}

Request::Request(const Request &copy)
{
	if (this != &copy)
	{
		// this->_method_str[::GET] = copy._method_str[::GET];
		// this->_method_str[::POST] = copy._method_str[::POST];
		// this->_method_str[::DELETE] = copy._method_str[::DELETE];
		// this->_method_str[::PUT] = copy._method_str[::PUT];
		// this->_method_str[::HEAD] = copy._method_str[::HEAD];
		this->_path = copy._path;
		this->_query = copy._query;
		this->_fragment = copy._fragment;
		this->_body_str = copy._body_str;
		this->_err_code = copy._err_code;
		this->_chunk_len = copy._chunk_len;
		this->_method = copy._method;
		this->_method_index = copy._method_index;
		this->_state = copy._state;
		this->_fields_done = copy._fields_done;
		this->_fbody = copy._fbody;
		this->_fbody_done = copy._fbody_done;
		this->_fchunked = copy._fchunked;
		this->_flag = copy._flag;
		this->_body_len = copy._body_len;
		this->_storage = copy._storage;
		this->_key_storage = copy._key_storage;
		this->_boundary = copy._boundary;
	}
	return ;
}

Request &Request::operator=(const Request &copy)
{
	if (this != &copy)
	{
		// this->_method_str[::GET] = copy._method_str[::GET];
		// this->_method_str[::POST] = copy._method_str[::POST];
		// this->_method_str[::DELETE] = copy._method_str[::DELETE];
		// this->_method_str[::PUT] = copy._method_str[::PUT];
		// this->_method_str[::HEAD] = copy._method_str[::HEAD];
		this->_path = copy._path;
		this->_query = copy._query;
		this->_fragment = copy._fragment;
		this->_body_str = copy._body_str;
		this->_err_code = copy._err_code;
		this->_chunk_len = copy._chunk_len;
		this->_method = copy._method;
		this->_method_index = copy._method_index;
		this->_state = copy._state;
		this->_fields_done = copy._fields_done;
		this->_fbody = copy._fbody;
		this->_fbody_done = copy._fbody_done;
		this->_fchunked = copy._fchunked;
		this->_flag = copy._flag;
		this->_body_len = copy._body_len;
		this->_storage = copy._storage;
		this->_key_storage = copy._key_storage;
		this->_boundary = copy._boundary;
	}
	return *this;
}

Method &Request::getMethod()
{
	return _method;
}

STR &Request::getPath()
{
	return _path;
}

STR &Request::getQuery()
{
	return _query;
}

STR &Request::getFragment()
{
	return _fragment;
}

STR Request::getHeader(const STR &name)
{
	MAP<STR, STR>::const_iterator it = _request_headers.find(name);
	if (it != _request_headers.end())
	{
		return it->second;
	}
	return "";
}

const MAP<STR, STR> &Request::getHeaders() const
{
	return _request_headers;
}

STR Request::getMethodStr()
{
	switch (_method)
	{
		case GET:
			return "GET";
		case POST:
			return "POST";
		case DELETE:
			return "DELETE";
		case PUT:
			return "PUT";
		case HEAD:
			return "HEAD";
		default:
			return "";
	}
}

STR &Request::getBody()
{
	return _body_str;
}

STR Request::getServerName()
{
	return _server_name;
}

bool Request::getFlag()
{
	return _flag;
}

STR &Request::getBoundary()
{
	return _boundary;
}

void Request::setBody(STR body)
{
	_body.clear();
	_body.insert(_body.begin(), body.begin(), body.end());
	_body_str = body;
}

void Request::setMethod(Method &method)
{
	_method = method;
}

void Request::setHeader(STR &name, STR &value)
{
	static const char* spaces = " \t";
	_storage.erase(0, _storage.find_first_not_of(spaces));
	_storage.erase(_storage.find_last_not_of(spaces) + 1);

	for(size_t i = 0; i < _storage.length(); ++i)
		_storage[i] = std::tolower(_storage[i]);
	_request_headers[_storage] = value;
}

void Request::setMaxBodySize(size_t size)
{
	_max_body_size = size;
}

void Request::printMessage()
{
	COUT << getMethodStr() << " " << _path << "?" << _query << "#" << _fragment << " HTTP/" << _ver_major << "." <<_ver_minor << ENDL;

	for (MAP<STR, STR>::iterator it = _request_headers.begin();
		it != _request_headers.end(); ++it)
	{
		COUT << it->first << ":" << it->second << ENDL;
	}

	for (VECTOR<u_int8_t>::iterator it = _body.begin(); it != _body.end(); ++it)
	{
		COUT << *it;
	}
	COUT << ENDL << "END OF BODY" << ENDL;

	COUT << "BODY FLAG = " << _fbody << ENDL;
	COUT << "BODY DONE FLAG = " << _fbody_done << ENDL;
	COUT << "FIELDS FLAG = " << _fields_done << ENDL;
}

void Request::_handleHeaders()
{
	std::stringstream ss;

	if (_request_headers.count("content-length"))
	{
		_fbody = true;
		ss << _request_headers["content-length"];
		ss >> _body_len;
	}
	if ( _request_headers.count("transfer-encoding"))
	{
		if (_request_headers["transfer-encoding"].find_first_of("chunked") != std::string::npos)
			_fchunked = true;
		_fbody = true;
	}
	if (_request_headers.count("host"))
	{
		size_t pos = _request_headers["host"].find_first_of(':');
		_server_name = _request_headers["host"].substr(0, pos);
	}
	if (_request_headers.count("content-type") && _request_headers["content-type"].find("multipart/form-data") != std::string::npos)
	{
		size_t pos = _request_headers["content-type"].find("boundary=", 0);
		if (pos != std::string::npos)
			this->_boundary = _request_headers["content-type"].substr(pos + 9, _request_headers["content-type"].size());
		this->_flag = true;
	}
	// COUT << "FBODYYYYY " << _fbody << ENDL;
	// COUT << "SERVER NAME  " << _server_name << ENDL;
}

short Request::errorCode()
{
	return _err_code;
}

void Request::clear()
{
	_path.clear();
	_query.clear();
	_fragment.clear();
	_storage.clear();
	_key_storage.clear();
	_request_headers.clear();
	_server_name.clear();
	_body.clear();
	_boundary.clear();
	_body_str = "";
	_method = NONE;
	_state = Request_Line;
	_body_len = 0;
	_err_code = 0;
	_chunk_len = 0x0;
	_method_index = 1;
	_fields_done = false;
	_fbody = false;
	_fbody_done = false;
	_fcompete = false;
	_fchunked = false;
	_flag = false;
}

bool Request::keepAlive()
{
	if (_request_headers.count("connection"))
	{
		if (_request_headers["connection"].find("close", 0) != STR::npos)
			return false;
	}
	return true;
}

void Request::cutReqBody(int bytes)
{
	_body_str = _body_str.substr(bytes);
}

bool Request::checkURIPos(const STR &path)
{
	STR tmp(path);
	char *res = strtok((char *)tmp.c_str(), "/");
	int pos = 0;

	while (res != NULL)
	{
		if (!strcmp(res, ".."))
			pos--;
		else
			pos++;
		
		if (pos < 0)
			return true;
		res = strtok(NULL, "/");
	}
	return false;
}

bool Request::allowedCharURI(u_int8_t ch)
{
	if ((ch >= '#' && ch <= ';') || (ch >= '?' && ch <= '[') || (ch >= 'a' && ch <= 'z') ||
		ch == '!' || ch == '=' || ch == ']' || ch == '_' || ch == '~')
		return true;

	return false;
}

bool Request::isToken(u_int8_t ch)
{
	if (ch == '!' || (ch >= '#' && ch <= '\'') || ch == '*' || ch == '+' || ch == '-' || ch == '.' ||
		(ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= '^' && ch <= '`') ||
		(ch >= 'a' && ch <= 'z') || ch == '|')
		return true;

	return false;
}

void Request::feed(char *data, size_t size)
{
	u_int8_t character;
	static std::stringstream s;

	for (size_t i = 0; i < size; ++i)
	{
		character = data[i];
		switch(_state)
		{
			case Request_Line:
			{
				if (character == 'G')
					_method = GET;
				else if (character == 'P')
				{
					_state = Request_Line_Post_Put;
					break;
				}
				else if (character == 'D')
					_method = DELETE;
				else if (character == 'H')
					_method = HEAD;
				else
				{
					_err_code = 501;
					COUT << "Method Error Reques_Line and Character is = " << character << ENDL;
					return ;
				}
				_state = Request_Line_Method;
				break;
			}

			case Request_Line_Post_Put:
			{
				if (character == 'O')
					_method = POST;
				else if (character == 'U')
					_method = PUT;
				else
				{
					_err_code = 501;
					COUT << "Method Error Reques_Line and Character is = " << character << ENDL;
					return ;
				}
				_method_index++;
				_state = Request_Line_Method;
				break;
			}

			case Request_Line_Method:
			{
				if (character == _method_str[_method][_method_index])
					_method_index++;
				else
				{
					_err_code = 501;
					COUT << "Method Error Reques_Line and Character is = " << character << ENDL;
					return ;
				}

				if ((size_t) _method_index == _method_str[_method].length())
					_state = Request_Line_Space;
				break;
			}

			case Request_Line_Space:
			{
				if (character != ' ')
				{
					_err_code = 400;
					COUT << "Request_Line_First_Space Error: Bad Character" << ENDL;
					return ;
				}
				_state = Request_Line_URI_Path_Slash;
				continue ;
			}

			case Request_Line_URI_Path_Slash:
			{
				if (character == '/')
				{
					_state = Request_Line_URI_Path;
					_storage.clear();
				}
				else
				{
					_err_code = 501;
					COUT << "Method Error Reques_Line and Character is = " << character << ENDL;
					return ;
				}
				break ;
			}

			case Request_Line_URI_Path:
			{
				if (character == ' ')
				{
					_state = Request_Line_Ver;
					_path.append(_storage);
					_storage.clear();
					continue ;
				}
				else if (character == '?')
				{
					_state = Request_Line_URI_Query;
					_path.append(_storage);
					_storage.clear();
					continue ;
				}
				else if (character == '#')
				{
					_state = Request_Line_URI_Fragment;
					_path.append(_storage);
					_storage.clear();
					continue ;
				}
				else if (!allowedCharURI(character))
				{
					_err_code = 400;
					COUT << "Request_Line_URI_Path Error: Bad character" << ENDL;
					return ;
				}
				else if (i > MAX_URI_LENGTH)
				{
					_err_code = 414;
					COUT << "Request_Line_URI_Path Error: URI Too long" << ENDL;
					return ;
				}
				break ;
			}

			case Request_Line_URI_Query:
			{
				if (character == ' ')
				{
					_state = Request_Line_Ver;
					_query.append(_storage);
					_storage.clear();
					continue ;
				}
				else if (character == '#')
				{
					_state = Request_Line_URI_Fragment;
					_path.append(_storage);
					_storage.clear();
					continue ;
				}
				else if (!allowedCharURI(character))
				{
					_err_code = 400;
					COUT << "Request_Line_URI_Query Error: Bad character" << ENDL;
					return ;
				}
				else if (i > MAX_URI_LENGTH)
				{
					_err_code = 414;
					COUT << "Request_Line_URI_Path Error: URI Too long" << ENDL;
					return ;
				}
				break ;
			}

			case Request_Line_URI_Fragment:
			{
				if (character == ' ')
				{
					_state = Request_Line_Ver;
					_query.append(_storage);
					_storage.clear();
					continue ;
				}
				else if (!allowedCharURI(character))
				{
					_err_code = 400;
					COUT << "Request_Line_URI_Fragment Error: Bad character" << ENDL;
					return ;
				}
				else if (i > MAX_URI_LENGTH)
				{
					_err_code = 414;
					COUT << "Request_Line_URI_Path Error: URI Too long" << ENDL;
					return ;
				}
				break ;
			}

			case Request_Line_Ver:
			{
				if (checkURIPos(_path))
				{
					_err_code = 400;
					COUT << "Request_URI_ERROR: goes before root" << ENDL;
					return ;
				}
				if (character != 'H')
				{
					_err_code = 400;
					COUT << "Request_Line_Ver Error: Bad Character" << ENDL;
					return ;
				}
				_state = Request_Line_HT;
				break ;
			}

			case Request_Line_HT:
			{
				if (character != 'T')
				{
					_err_code = 400;
					COUT << "Request_Line_HT: Bad Character" << ENDL;
					return ;
				}
				_state = Request_Line_HTT;
				break ;
			}

			case Request_Line_HTT:
			{
				if (character != 'T')
				{
					_err_code = 400;
					COUT << "Request_Line_HTT: Bad Character" << ENDL;
					return ;
				}
				_state = Request_Line_HTTP;
				break ;
			}

			case Request_Line_HTTP:
			{
				if (character != 'P')
				{
					_err_code = 400;
					COUT << "Request_Line_HTTP: Bad Character" << ENDL;
					return ;
				}
				_state = Request_Line_HTTP_Slash;
				break ;
			}

			case Request_Line_HTTP_Slash:
			{
				if (character != '/')
				{
					_err_code = 400;
					COUT << "Request_Line_HTTP_Slash: Bad Character" << ENDL;
					return ;
				}
				_state = Request_Line_Major;
				break ;
			}

			case Request_Line_Major:
			{
				if (!isdigit(character))
				{
					_err_code = 400;
					COUT << "Request_Line_Major: Bad Character" << ENDL;
					return ;
				}
				_ver_major = character;

				_state = Request_Line_Dot;
				break;
			}

			case Request_Line_Dot:
			{
				if(character != '.')
				{
					_err_code = 400;
					COUT << "Request_Line_Dot: Bad Character" << ENDL;
					return ;
				}
				_state = Request_Line_Minor;
				break;
			}

			case Request_Line_Minor:
			{
				if (!isdigit(character))
				{
					_err_code = 400;
					COUT << "Request_Line_Minor: Bad character" << ENDL;
					return ;
				}
				_ver_minor = character;
				_state = Request_Line_CR;
				break ;
			}

			case Request_Line_CR:
			{
				if (character != '\r')
				{
					_err_code = 400;
					COUT << "Request_Line_CR: Bad Character" << ENDL;
					return ; 
				}
				_state = Request_Line_LF;
				break ;
			}

			case Request_Line_LF:
			{
				if (character != '\n')
				{
					_err_code =  400;
					COUT << "Request_Line_LF Error: Bad Character" << ENDL;
					return ;
				}
				_state = Name_Start;
				_storage.clear();
				continue ;
			}

			case Name_Start:
			{
				if (character == '\r')
					_state = Name_End;
				else if (isToken(character))
					_state = Name;
				else
				{
					_err_code = 400;
					COUT << "Name_Start Error: Bad character" << ENDL;
					return ;
				}
				break ;
			}

			case Name_End:
			{
				if (character == '\n')
				{
					_storage.clear();
					_fields_done = true;
					_handleHeaders();

					if (_fbody == 1)
					{
						if (_fchunked == true)
							_state = Chunked_Len_Begin;
						else
							_state = Message_Body;
					}
					else
						_state = Parsing_Done;
					continue;
				}
				else
				{
					_err_code = 400;
					COUT << "Name_End Error: BAd character" << ENDL;
					return ;
				}
				break ;
			}

			case Name:
			{
				if (character == ':')
				{
					_key_storage = _storage;
					_storage.clear();
					_state = Value;
					continue ;
				}
				else if (!isToken(character))
				{
					_err_code = 400;
					COUT << "Name Error: Bad character" << ENDL;
					return ;
				}
				break ;
			}

			case Value:
			{
				if (character == '\r')
				{
					setHeader(_key_storage, _storage);
					_key_storage.clear();
					_storage.clear();
					_state = Value_End;
					continue ;
				}
				break;
			}

			case Value_End:
			{
				if (character == '\n')
				{
					_state = Name_Start;
					continue ;
				}
				else
				{
					_err_code = 400;
					COUT << "Value End Error: Bad Character" << ENDL;
					return ;
				}
				break ;
			}

			case Chunked_Len_Begin:
			{
				if (isxdigit(character) == 0)
				{
					_err_code = 400;
					COUT << "Chunked_Len_Begin Error: Bad character" << ENDL;
					return ;
				}
				s.str("");
				s.clear();
				s << character;
				s >> HEX >> _chunk_len;
				if (_chunk_len == 0)
					_state = Chunked_Len_CR;
				else
					_state = Chunked_Len;
				continue ;
			}

			case Chunked_Len:
			{
				if (isxdigit(character) != 0)
				{
					int temp_len = 0;
					s.str("");
					s.clear();
					s << character;
					s >> HEX >> temp_len;
					_chunk_len *= 16;
					_chunk_len += temp_len;
				}
				else if (character == '\r')
					_state = Chunked_Len_LF;
				else
					_state = Chunked_Ignore;
				continue ;
			}

			case Chunked_Len_CR:
			{
				if ( character == '\r')
					_state = Chunked_Len_LF;
				else
				{
					_err_code = 400;
					COUT << "Bad Character (Chunked_Length_CR)" << ENDL;
					return ;
				}
				continue ;
			}

			case Chunked_Len_LF:
			{
				if (character == '\n')
				{
					if (_chunk_len == 0)
						_state = Chunked_End_CR;
					else
						_state = Chunked_Data;
				}
				else
				{
					_err_code = 400;
					COUT << "Chunked_Len_LF Error: Bad character" << ENDL;
					return ;
				}
				continue ;
			}

			case Chunked_Ignore:
			{
				if (character == '\r')
					_state = Chunked_Len_LF;
				continue ;
			}
		
			case Chunked_Data:
			{
				_body.push_back(character);
				--_chunk_len;
				if (_chunk_len == 0)
					_state = Chunked_Data_CR;
				continue ;
			}

			case Chunked_Data_CR:
			{
				if (character == '\r')
					_state = Chunked_Data_LF;
				else
				{
					_err_code = 400;
					COUT << "Chunked_Data_CR Error: Bad character" << ENDL;
					return ;
				}
				continue ;
			}

			case Chunked_Data_LF:
			{
				if (character == '\n')
					_state = Chunked_Len_Begin;
				else
				{
					_err_code = 400;
					COUT << "Chunked_End_CR Error: Bad Character" << ENDL;
					return ;
				}
				continue ;
			}

			case Chunked_End_CR:
			{
				if (character != '\r')
				{
					_err_code = 400;
					COUT << "Chunked_End_CR Error: Bad character" << ENDL;
					return ;
				}
				_state = Chunked_End_LF;
				continue ;
			}

			case Chunked_End_LF:
			{
				if (character != '\n')
				{
					_err_code = 400;
					COUT << "Chunked_End_LF Error: Bad character" << ENDL;
					return ;
				}
				_fbody_done = true;
				_state = Parsing_Done;
				continue ;
			}
			
			case Message_Body:
			{
				if (_body.size() < _body_len)
					_body.push_back(character);
				if (_body.size() == _body_len)
				{
					_fbody_done = true;
					_state = Parsing_Done;
				}
				break ;
			}
			case Parsing_Done:
			{
				return ;
			}
		}
		_storage += character;
	}
	if (_state == Parsing_Done)
	{
		_body_str.append((char *)_body.data(), _body.size());
	}
}

bool Request::parsingDone()
{
	return _state == Parsing_Done;
}