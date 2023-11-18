#include "../../include/Request.hpp"

void Request::parseRequestLine(u_int8_t character)
{
	if (character == 'G')
		_method = GET;
	else if (character == 'P')
	{
		_state = Request_Line_Post_Put;
		_method_index++;
	}
	else if (character == 'D')
		_method = DELETE;
	else if (character == 'H')
		_method = HEAD;
	else
	{
		_err_code = 501;
		COUT << "Method Error Reques_Line and Character is = " << character << ENDL;
		_state = Parsing_Done;
		return;
	}
	_state = Request_Line_Method;
}

void Request::parseRequestLinePostPut(u_int8_t character)
{
	if (character == 'O')
		_method = POST;
	else if (character == 'U')
		_method = PUT;
	else
	{
		_err_code = 501;
		COUT << "Method Error Reques_Line and Character is = " << character << ENDL;
		_state = Parsing_Done;
	}
	_method_index++;
	_state = Request_Line_Method;
}

void Request::parseRequestLineMethod(u_int8_t character)
{
	if (character == _method_str[_method][_method_index])
		_method_index++;
	else
	{
		_err_code = 501;
		COUT << "Method Error Reques_Line and Character is = " << character << ENDL;
		_state = Parsing_Done;
	}

	if ((size_t) _method_index == _method_str[_method].length())
		_state = Request_Line_Space;
}

void Request::parseRequestLineFirstSpace(u_int8_t character)
{
	if (character != ' ')
	{
		_err_code = 400;
		COUT << "Request_Line_First_Space Error: Bad Character" << ENDL;
		_state = Parsing_Done;
	}
	_state = Request_Line_URI_Path_Slash;	
}

void Request::parseRequestLineURIPathSlash(u_int8_t character)
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
		_state = Parsing_Done;
	}
}

void Request::parseRequestLineURIPath(u_int8_t character, size_t i)
{
	if (character == ' ')
	{
		_state = Request_Line_Ver;
		if (!_storage.empty())
		{	
			_path.append("/" + _storage);
			_storage.clear();
		}
	}
	else if (character == '?')
	{
		_state = Request_Line_URI_Query;
		if (!_storage.empty())
		{	
			_path.append("/" + _storage);
			_storage.clear();
		}
	}
	else if (character == '#')
	{
		_state = Request_Line_URI_Fragment;
		if (!_storage.empty())
		{	
			_path.append("/" + _storage);
			_storage.clear();
		}
	}
	else if (!allowedCharURI(character))
	{
		_err_code = 400;
		COUT << "Request_Line_URI_Path Error: Bad character" << ENDL;
		_state = Parsing_Done;
	}
	else if (i > MAX_URI_LENGTH)
	{
		_err_code = 414;
		COUT << "Request_Line_URI_Path Error: URI Too long" << ENDL;
		_state = Parsing_Done;
	}
	_storage += character;
}

void Request::parseRequestLineURIQuery(u_int8_t character, size_t i)
{
	if (character == ' ')
	{
		_state = Request_Line_Ver;
		_query.append(_storage);
		_storage.clear();
	}
	else if (character == '#')
	{
		_state = Request_Line_URI_Fragment;
		_path.append(_storage);
		_storage.clear();
	}
	else if (!allowedCharURI(character))
	{
		_err_code = 400;
		COUT << "Request_Line_URI_Query Error: Bad character" << ENDL;
		_state = Parsing_Done;
	}
	else if (i > MAX_URI_LENGTH)
	{
		_err_code = 414;
		COUT << "Request_Line_URI_Path Error: URI Too long" << ENDL;
		_state = Parsing_Done;
	}
}

void Request::parseRequestLineURIFragment(u_int8_t character, size_t i)
{
	if (character == ' ')
	{
		_state = Request_Line_Ver;
		_query.append(_storage);
		_storage.clear();
	}
	else if (!allowedCharURI(character))
	{
		_err_code = 400;
		COUT << "Request_Line_URI_Fragment Error: Bad character" << ENDL;
		_state = Parsing_Done;
	}
	else if (i > MAX_URI_LENGTH)
	{
		_err_code = 414;
		COUT << "Request_Line_URI_Path Error: URI Too long" << ENDL;
		_state = Parsing_Done;
	}
}

void Request::parseRequestLineVer(u_int8_t character)
{
	if (checkURIPos(_path))
	{
		_err_code = 400;
		COUT << "Request_URI_ERROR: goes before root" << ENDL;
		_state = Parsing_Done;
	}
	if (character != 'H')
	{
		_err_code = 400;
		COUT << "Request_Line_Ver Error: Bad Character" << ENDL;
		_state = Parsing_Done;
	}
	_state = Request_Line_HT;
}

void Request::parseRequestLineHT(u_int8_t character)
{
	if (character != 'T')
	{
		_err_code = 400;
		COUT << "Request_Line_HT: Bad Character" << ENDL;
		_state = Parsing_Done;
	}
	_state = Request_Line_HTT;
}

void Request::parseRequestLineHTT(u_int8_t character)
{
	if (character != 'T')
	{
		_err_code = 400;
		COUT << "Request_Line_HTT: Bad Character" << ENDL;
		_state = Parsing_Done;
	}
	_state = Request_Line_HTTP;
}

void Request::parseRequestLineHTTP(u_int8_t character)
{
	if (character != 'P')
	{
		_err_code = 400;
		COUT << "Request_Line_HTTP: Bad Character" << ENDL;
		_state = Parsing_Done;
	}
	_state = Request_Line_HTTP_Slash;
}

void Request::parseRequestLineHTTPSlash(u_int8_t character)
{
	if (character != '/')
	{
		_err_code = 400;
		COUT << "Request_Line_HTTP_Slash: Bad Character" << ENDL;
		_state = Parsing_Done;
	}
	_state = Request_Line_Major;
}

void Request::parseRequestLineMajor(u_int8_t character)
{
	if (!isdigit(character))
	{
		_err_code = 400;
		COUT << "Request_Line_Major: Bad Character" << ENDL;
		_state = Parsing_Done;
	}
	_ver_major = character;

	_state = Request_Line_Dot;
}

void Request::parseRequestLineDot(u_int8_t character)
{
	if(character != '.')
	{
		_err_code = 400;
		COUT << "Request_Line_Dot: Bad Character" << ENDL;
		_state = Parsing_Done;
	}
	_state = Request_Line_Minor;
}

void Request::parseRequestLineMinor(u_int8_t character)
{
	if (!isdigit(character))
	{
		_err_code = 400;
		COUT << "Request_Line_Minor: Bad character" << ENDL;
		_state = Parsing_Done;
	}
	_ver_minor = character;
	_state = Request_Line_CR;
}

void Request::parseRequestLineCR(u_int8_t character)
{
	if (character != '\r')
	{
		_err_code = 400;
		COUT << "Request_Line_CR: Bad Character" << ENDL;
		_state = Parsing_Done;
	}
	_state = Request_Line_LF;
}

void Request::parseRequestLineLF(u_int8_t character)
{				
	if (character != '\n')
	{
		_err_code =  400;
		COUT << "Request_Line_LF Error: Bad Character" << ENDL;
		_state = Parsing_Done;
	}
	_state = Name_Start;
	_storage.clear();
}

void Request::parseNameStart(u_int8_t character)
{
	if (character == '\r')
		_state = Name_End;
	else if (isToken(character))
	{	
		_state = Name;
		_key_storage += character;
	}
	else
	{
		_err_code = 400;
		COUT << "Name_Start Error: Bad character" << ENDL;
		_state = Parsing_Done;
	}
}

void Request::parseNameEnd(u_int8_t character)
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
	}
	else
	{
		_err_code = 400;
		COUT << "Name_End Error: BAd character" << ENDL;
		_state = Parsing_Done;
	}
}

void Request::parseName(u_int8_t character)
{
	if (character == ':')
	{
		_state = Value;
	}
	else if (isToken(character))
	{
		_key_storage += character;
	}
	else
    {
        _err_code = 400;
        COUT << "Name Error: Bad character" << ENDL;
        _state = Parsing_Done;
    }
}

void Request::parseValue(u_int8_t character)
{
	if (character == ' ')
    {
        // Ignore leading spaces in the header value.
    }
	if (character == '\r')
	{
		setHeader(_key_storage, _storage);
		_key_storage.clear();
		_storage.clear();
		_state = Value_End;
	}
	else
	{
		_storage += character;
	}
}

void Request::parseValueEnd(u_int8_t character)
{
	if (character == '\n')
	{
		_state = Name_Start;
	}
	else
	{
		_err_code = 400;
		COUT << "Value End Error: Bad Character" << ENDL;
		_state = Parsing_Done;
	}
}

void Request::parseChunkedLengthBegin(u_int8_t character)
{
	if (isxdigit(character) )
	{
		std::stringstream s;
		s.str("");
		s.clear();
		s << character;
		s >> HEX >> _chunk_len;
		if (_chunk_len == 0)
			_state = Chunked_Len_CR;
		else
			_state = Chunked_Len;
	}
	else
	{
		_err_code = 400;
		COUT << "Chunked_Len_Begin Error: Bad character" << ENDL;
		_state = Parsing_Done;
	}
}

void Request::parseChunkedLength(u_int8_t character)
{
	if (isxdigit(character) != 0)
	{
		std::stringstream s;
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
}

void Request::parseChunkedLengthCR(u_int8_t character)
{
	if ( character == '\r')
		_state = Chunked_Len_LF;
	else
	{
		_err_code = 400;
		COUT << "Bad Character (Chunked_Length_CR)" << ENDL;
		_state = Parsing_Done;
	}
}

void Request::parseChunkedLengthLF(u_int8_t character)
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
		_state = Parsing_Done;
	}
}

void Request::parseChunkedIgnore(u_int8_t character)
{
	if (character == '\r')
		_state = Chunked_Len_LF;
}

void Request::parseChunkedData(u_int8_t character)
{
	_body.push_back(character);
	--_chunk_len;
	if (_chunk_len == 0)
		_state = Chunked_Data_CR;
}

void Request::parseChunkedDataCR(u_int8_t character)
{
	if (character == '\r')
		_state = Chunked_Data_LF;
	else
	{
		_err_code = 400;
		COUT << "Chunked_Data_CR Error: Bad character" << ENDL;
		_state = Parsing_Done;
	}
}

void Request::parseChunkedDataLF(u_int8_t character)
{
	if (character == '\n')
		_state = Chunked_Len_Begin;
	else
	{
		_err_code = 400;
		COUT << "Chunked_End_CR Error: Bad Character" << ENDL;
		_state = Parsing_Done;
	}
}

void Request::parseChunkedEndCR(u_int8_t character)
{
	if (character != '\r')
	{
		_err_code = 400;
		COUT << "Chunked_End_CR Error: Bad character" << ENDL;
		_state = Parsing_Done;
	}
	_state = Chunked_End_LF;
}

void Request::parseChunkedEndLF(u_int8_t character)
{
	if (character != '\n')
	{
		_err_code = 400;
		COUT << "Chunked_End_LF Error: Bad character" << ENDL;
		_state = Parsing_Done;
	}
	_fbody_done = true;
	_state = Parsing_Done;
}

void Request::parseMessageBody(u_int8_t character)
{
	if (_body.size() < _body_len)
		_body.push_back(character);
	if (_body.size() == _body_len)
	{
		_fbody_done = true;
		_state = Parsing_Done;
	}
}
