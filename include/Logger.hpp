#ifndef LOGGER_HPP
# define LOGGER_HPP

# include "Webserv.hpp"

# define TIME_ZONE	2;

enum LogPrio
{
	DEBUG,
	INFO,
	ERROR
};

enum LogState
{
	ON,
	OFF
};

enum Mode
{
	CONSOLE_OUTPUT,
	FILE_OUTPUT
};

class Logger
{
	public:
		static STR					file_name;
		static LogPrio				prio;
		static MAP<LogPrio, STR>	string_prio;
		static LogState				current_state;

		static void					setFileName(STR);
		static void					setState(LogState);
		static void					setPrio(LogPrio);

		static void					messageLog(const char *, Mode, const char*, ...);
		static void					startFileLog();
		static STR					getCurrentTime();

	private:
		static MAP<LogPrio, STR>	mapInitialization();
};

#endif
