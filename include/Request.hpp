#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Webserv.hpp"
# include "Utils.hpp"

enum Method
{
	GET,
	POST,
	DELETE,
	PUT,
	HEAD,
	NONE
};

enum ParsingState
{
	Request_Line,
	Request_Line_Post_Put,
	Request_Line_Method,
	Request_Line_Space,
	Request_Line_URI_Path_Slash,
	Request_Line_URI_Path,
	Request_Line_URI_Query,
	Request_Line_URI_Fragment,
	Request_Line_Ver,
	Request_Line_HT,
	Request_Line_HTT,
	Request_Line_HTTP,
	Request_Line_HTTP_Slash,
	Request_Line_Major,
	Request_Line_Dot,
	Request_Line_Minor,
	Request_Line_CR,
	Request_Line_LF,
	Name_Start,
	Name_End,
	Name,
	Value,
	Value_End,
	Chunked_Len_Begin,
	Chunked_Len,
	Chunked_Ignore,
	Chunked_Len_CR,
	Chunked_Len_LF,
	Chunked_Data,
	Chunked_Data_CR,
	Chunked_Data_LF,
	Chunked_End_CR,
	Chunked_End_LF,
	Message_Body,
	Parsing_Done
};

class Request
{
	public:
		Request();
		~Request();
		Request(const Request &copy);
		Request &operator=(const Request &copy);

		Method		&getMethod();
		STR			&getPath();
		STR			&getQuery();
		STR			&getFragment();
		STR			getHeader(STR const &);
		const MAP<STR, STR>	&getHeaders() const;
		STR			getMethodStr();
		STR			&getBody();
		STR			getServerName();
		STR			&getBoundary();
		bool		getFlag();

		void		setMethod(Method &);
		void		setHeader(STR &, STR &);
		void		setMaxBodySize(size_t);
		void		setBody(STR name);

		void		feed(char *data, size_t size);
		bool		parsingDone();
		void		printMessage();
		void		clear();
		short		errorCode();
		bool		keepAlive();
		void		cutReqBody(int bytes);
		bool		checkURIPos(const STR &path);
		bool		allowedCharURI(u_int8_t ch);
		bool		isToken(u_int8_t ch);

		void				parseRequestLine(u_int8_t character);
		void				parseRequestLinePostPut(u_int8_t character);
		void				parseRequestLineMethod(u_int8_t character);
		void				parseRequestLineFirstSpace(u_int8_t character);
		void				parseRequestLineURIPathSlash(u_int8_t character);
		void				parseRequestLineURIPath(u_int8_t character, size_t i);
		void				parseRequestLineURIQuery(u_int8_t character, size_t i);
		void				parseRequestLineURIFragment(u_int8_t character, size_t i);
		void				parseRequestLineVer(u_int8_t character);
		void				parseRequestLineHT(u_int8_t character);
		void				parseRequestLineHTT(u_int8_t character);
		void				parseRequestLineHTTP(u_int8_t character);
		void				parseRequestLineHTTPSlash(u_int8_t character);
		void				parseRequestLineMajor(u_int8_t character);
		void				parseRequestLineDot(u_int8_t character);
		void				parseRequestLineMinor(u_int8_t character);
		void				parseRequestLineCR(u_int8_t character);
		void				parseRequestLineLF(u_int8_t character);
		void				parseNameStart(u_int8_t character);
		void				parseNameEnd(u_int8_t character);
		void				parseName(u_int8_t character);
		void				parseValue(u_int8_t character);
		void				parseValueEnd(u_int8_t character);
		void				parseChunkedLengthBegin(u_int8_t character);
		void				parseChunkedLength(u_int8_t character);
		void				parseChunkedLengthCR(u_int8_t character);
		void				parseChunkedLengthLF(u_int8_t character);
		void				parseChunkedIgnore(u_int8_t character);
		void				parseChunkedData(u_int8_t character);
		void				parseChunkedDataCR(u_int8_t character);
		void				parseChunkedEndCR(u_int8_t character);
		void				parseChunkedDataLF(u_int8_t character);
		void				parseChunkedEndLF(u_int8_t character);
		void				parseMessageBody(u_int8_t character);

	private:
		STR					_path;
		STR 				_query;
		STR					_fragment;
		MAP<STR, STR>					_request_headers;
		VECTOR<u_int8_t>	_body;
		STR					_boundary;
		Method				_method;
		MAP<u_int8_t, STR>	_method_str;
		ParsingState		_state;
		size_t				_max_body_size;
		size_t				_body_len;
		short				_err_code;
		size_t				_chunk_len;
		STR					_storage;
		STR					_key_storage;
		short				_method_index;
		u_int8_t			_ver_major;
		u_int8_t			_ver_minor;
		STR					_server_name;
		STR					_body_str;

		bool				_fields_done;
		bool				_fbody;
		bool				_fbody_done;
		bool				_fcompete;
		bool				_fchunked;
		bool				_fmultiform;

		void				_handleHeaders();
};

#endif
