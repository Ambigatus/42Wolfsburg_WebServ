/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_Configuration.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboichuk <hboichuk@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 17:45:13 by hboichuk          #+#    #+#             */
/*   Updated: 2023/12/20 18:21:02 by hboichuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_CONFIGURATION_HPP
# define CGI_CONFIGURATION_HPP

# include "Webserv.hpp"
# include "Request.hpp"

class Request;
class CGIConfig
{
	private:
		MAP<STR, STR>		_env;
		char**				_char_env;
		char**				_argv;
		int					_exit_status;
		STR					_cgi_path;
		pid_t				_cgi_pid;

	public:
		int					pipe_in[2];
		int					pipe_out[2];

	CGIConfig();
	CGIConfig(STR path);
	~CGIConfig();
	CGIConfig(CGIConfig const &copy);
	CGIConfig &operator=(CGIConfig const &copy);

	void	envInitializatation(Request& request, const VECTOR<Location>::iterator iter_loc);
	void	CGIEnvInitialization(Request& request, const VECTOR<Location>::iterator iter_loc);
	void	execute(short &error_code);
	void	sendPage(int &pipe_out, int &fd, STR &);
	void	fixHeader(STR &header);
	void	cleaning();
	STR		cookies(const STR &str);

	void	setCGIpid(pid_t cgi_pid);
	void	setCGIpath(const STR &cgi_path);

	const MAP<STR, STR> &getEnv() const;
	const pid_t			&getCGIpid() const;
	const STR			&getCGIpath() const;

	STR					getAfter(const STR &path, char delim);
	STR					getBefore(const STR &path, char delim);
	STR					getPathInfo(STR &path, VECTOR<STR> extensions);
	int					cookiesCounter(const STR &str);
	int					startFinder(const STR path, const STR delim);
	STR					decode(STR &path);
};

#endif
