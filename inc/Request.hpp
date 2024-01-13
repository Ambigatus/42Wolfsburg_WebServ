#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Webserv.hpp"

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
    Request_Line_First_Space,
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
    Field_Name_Start,
    Fields_End,
    Field_Name,
    Field_Value,
    Field_Value_End,
    Chunked_Length_Begin,
    Chunked_Length,
    Chunked_Ignore,
    Chunked_Length_CR,
    Chunked_Length_LF,
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

        Method                              &getMethod();
        STR                                 &getPath();
        STR                                 &getQuery();
        STR                                 &getFragment();
        STR                                 getHeader(STR const &);
		const MAP<STR, STR>                 &getHeaders() const;
		STR                                 getMethodStr();
        STR                                 &getBody();
        STR                                 getServerName();
        STR                                 &getBoundary();
        bool                                getMultiformFlag();
        
        void        setMethod(Method &);
        void        setHeader(STR &, STR &);
        void        setMaxBodySize(size_t);
        void        setBody(STR name);

        void        methodCheck(char *data, size_t size);
        bool        parsingCompleted();
        void        printMessage();
        void        clear();
        short       errorCode();
        bool        keepAlive();
        void        cutReqBody(int bytes);

    private:
        STR                                 _path;
        STR                                 _query;
        STR                                 _fragment;
        MAP<STR, STR>                       _request_headers;
        VECTOR<u_int8_t>                    _body;
        STR                                 _boundary;
        Method                              _method;
        MAP<u_int8_t, STR>                  _method_str;
        ParsingState                        _state;
        size_t                              _max_body_size;
        size_t                              _body_length;
        short                               _error_code;
        size_t                              _chunk_length;
        STR                                 _storage;
        STR                                 _key_storage;
        short                               _method_index;
        u_int8_t                            _ver_major;
        u_int8_t                            _ver_minor;
        STR                                 _server_name;
        STR                                 _body_str;
        /* flags */
        bool                                _fields_done_flag;
        bool                                _body_flag;
        bool                                _body_done_flag;
        bool                                _complete_flag;
        bool                                _chunked_flag;
        bool                                _multiform_flag;

        void                                _handle_headers();

};

#endif
