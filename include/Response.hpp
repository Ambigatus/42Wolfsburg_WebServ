#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Webserv.hpp"
# include "Request.hpp"
# include "ServerConfiguration.hpp"
# include "CGI_Configuration.hpp"
# include "ConfigurationFile.hpp"
# include "ConfigurationParser.hpp"

class Response
{
	public:
		Response();
		Response(Request&);
		~Response();
		Response(const Response &copy);
		Response &operator=(const Response &copy);

		STR		getResponse();
		size_t	getLen()const;
		int		getCode()const;

		void	setRequest(Request &);
		void	setServer(ServerConfiguration &);

		void	buildResponse();
		void	clear();
		void	handleCGI(Request &);
		void	cutResponse(size_t);
		int		getCgiState();
		void	setCgiState(int);
		void	setErrorResponse(short code);
		STR		removeBoundary(STR &body, STR &boundary);
		
		STR			_response_ready;
		CGIConfig	_cgi_conf;
		Request		_request;
		static const std::unordered_map<STR, STR>	mimeTypes;

	private:
		ServerConfiguration			_server;
		STR							_target_file;
		VECTOR<uint8_t>						_body;
		size_t						_body_len;
		STR							_response_body;
		STR							_location;
		short						_code;
		char						*_res;
		int							_cgi;
		int							_cgi_fd[2];
		size_t						_cgi_response_len;
		bool						_auto_index;

		int					buildBody();
		size_t				fileSize();
		int					readFile();
		void				setStatusLine();
		void				setHeaders();
		void				setServerErrorPages();
		void				addMimeType();
		void				addContentLen();
		void				addConnection();
		void				addServer();
		void				addLocation();
		void				addDate();
		int					handleTarget();
		void				buildErrorBody();
		bool				reqError();
		int					handleCgi(STR &);
		int					handleCgiTemp(STR &);
};

#endif