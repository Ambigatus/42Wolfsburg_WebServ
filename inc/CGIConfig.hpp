#ifndef CGICONFIG_HPP
#define CGICONFIG_HPP

#include "Webserv.hpp"

class Request;
class CGIConfig {
	private:
		MAP<STR, STR>	                    _env;
		char**								_ch_env;
		char**								_argv;
		int									_exit_status;
		STR							        _cgi_path;
		pid_t								_cgi_pid;

	public:
		int	pipe_in[2];
		int	pipe_out[2];

		CGIConfig();
		CGIConfig(STR path);
		~CGIConfig();
		CGIConfig(CGIConfig const &other);
		CGIConfig &operator=(CGIConfig const &rhs);

		void EnvInitialization(Request& req, const VECTOR<Location>::iterator it_loc);
		void EnvCgiInitialization(Request& req, const VECTOR<Location>::iterator it_loc);
		void execute(short &error_code);
		void sendHeaderBody(int &pipe_out, int &fd, STR &);
		void fixHeader(STR &header);
		void clear();
		STR setCookie(const STR& str);

		void setCgiPid(pid_t cgi_pid);
		void setCgiPath(const STR &cgi_path);

		const std::map<STR, STR> &getEnv() const;
		const pid_t &getCgiPid() const;
		const STR &getCgiPath() const;

		STR	getAfter(const std::string& path, char delim);
		STR	getBefore(const std::string& path, char delim);
		STR	getPathInfo(std::string& path, std::vector<std::string> extensions);
		int	countCookies(const std::string& str);
		int startFinder(const std::string path, const std::string delim);
		STR decode(std::string &path);
};

#endif
