#include "../../include/Request.hpp"

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
	  _fmultiform(false)
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
		this->_fmultiform = copy._fmultiform;
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
		this->_fmultiform = copy._fmultiform;
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

STR Request::getHeader(STR const &name)
{
	MAP<STR, STR>::const_iterator it = _request_headers.find(name);
	if (it != _request_headers.end())
	{
		return it->second;
	}
	return _request_headers[name];
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
	return _fmultiform;
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
	_request_headers[name] = value;
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
		if (_request_headers["transfer-encoding"].find_first_of("chunked") != STR::npos)
			_fchunked = true;
		_fbody = true;
	}
	if (_request_headers.count("host"))
	{
		size_t pos = _request_headers["host"].find_first_of(':');
		_server_name = _request_headers["host"].substr(0, pos);
	}
	if (_request_headers.count("content-type") && _request_headers["content-type"].find("multipart/form-data") != STR::npos)
	{
		size_t pos = _request_headers["content-type"].find("boundary=", 0);
		if (pos != STR::npos)
			this->_boundary = _request_headers["content-type"].substr(pos + 9, _request_headers["content-type"].size());
		this->_fmultiform = true;
	}
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
	_fmultiform = false;
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
	for (size_t i = 0; i < size; ++i)
	{
		u_int8_t character = data[i];

		switch (_state)
		{
			case Request_Line:
				parseRequestLine(character);
				break;
			case Request_Line_Post_Put:
				parseRequestLinePostPut(character);
				break;
			case Request_Line_Method:
				parseRequestLineMethod(character);
				break;
			case Request_Line_Space:
				parseRequestLineFirstSpace(character);
				break;
			case Request_Line_URI_Path_Slash:
				parseRequestLineURIPathSlash(character);
				break;
			case Request_Line_URI_Path:
				parseRequestLineURIPath(character, i);
				break;
			case Request_Line_URI_Query:
				parseRequestLineURIQuery(character, i);
				break;
			case Request_Line_URI_Fragment:
				parseRequestLineURIFragment(character, i);
				break;
			case Request_Line_Ver:
				parseRequestLineVer(character);
				break;
			case Request_Line_HT:
				parseRequestLineHT(character);
				break;
			case Request_Line_HTT:
				parseRequestLineHTT(character);
				break;
			case Request_Line_HTTP:
				parseRequestLineHTTP(character);
				break;
			case Request_Line_HTTP_Slash:
				parseRequestLineHTTPSlash(character);
				break;
			case Request_Line_Major:
				parseRequestLineMajor(character);
				break;
			case Request_Line_Dot:
				parseRequestLineDot(character);
				break;
			case Request_Line_Minor:
				parseRequestLineMinor(character);
				break;
			case Request_Line_CR:
				parseRequestLineCR(character);
				break;
			case Request_Line_LF:
				parseRequestLineLF(character);
				break;
			case Name_Start:
				parseNameStart(character);
				break;
			case Name_End:
				parseNameEnd(character);
				break;
			case Name:
				parseName(character);
				break;
			case Value:
				parseValue(character);
				break;
			case Value_End:
				parseValueEnd(character);
				break;
			case Chunked_Len_Begin:
				parseChunkedLengthBegin(character);
				break;
			case Chunked_Len:
				parseChunkedLength(character);
				break;
			case Chunked_Len_CR:
				parseChunkedLengthCR(character);
				break;
			case Chunked_Len_LF:
				parseChunkedLengthLF(character);
				break;
			case Chunked_Ignore:
				parseChunkedIgnore(character);
				break;
			case Chunked_Data:
				parseChunkedData(character);
				break;
			case Chunked_Data_CR:
				parseChunkedDataCR(character);
				break;
			case Chunked_Data_LF:
				parseChunkedDataLF(character);
				break;
			case Chunked_End_CR:
				parseChunkedEndCR(character);
				break;
			case Chunked_End_LF:
				parseChunkedEndLF(character);
				break;
			case Message_Body:
				parseMessageBody(character);
				break;
			case Parsing_Done:
				return;
		}

		// _storage += character;
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
