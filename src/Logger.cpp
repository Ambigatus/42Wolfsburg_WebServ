#include "../../include/Logger.hpp"

STR Logger::file_name = "logfile.txt";
LogPrio Logger::prio = ERROR;
LogState Logger::current_state = ON;

MAP<LogPrio, STR> Logger::string_prio = mapInitialization();

/*This function is responsible for initializing and returning a map that associates log 
priorities (LogPrio) with their corresponding log labels as strings. function initializes 
and returns a map that helps in associating log priorities with their respective labels, 
making it easier to format log messages with the appropriate log level indicators.*/
MAP<LogPrio, STR> Logger::mapInitialization()
{
	MAP<LogPrio, STR>	info_map;

	info_map[DEBUG] = "[DEBUG]      ";
	info_map[INFO] = "[INFO]      ";
	info_map[ERROR] = "[ERROR]      ";
	return info_map;
}

/*This function is responsible for logging messages with different output modes 
(file or console) and applying color to console output. The function formats and 
logs messages based on the specified mode (file or console). It applies color to 
console output and includes a timestamp in both file and console logs when logging 
is enabled.*/
void	Logger::messageLog(const char *color, Mode mode, const char* msg, ...)
{
	char		output[8192];
	va_list		args;
	int			temp;

	if (current_state == ON)
	{
		va_start(args, msg);
		temp = vsnprintf(output, 8192, msg, args);
		STR time = getCurrentTime();
		if (mode == FILE_OUTPUT)
		{
			if (mkdir("logs", 0777) < 0 && errno != EEXIST)
			{
				CERR << "ERROR: cant create a folder(mkdir). Details: " << strerror(errno) << ENDL;
				return ;
			}
			int fd = open(("logs/" + file_name).c_str(), O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR);
			COUT << "File descriptor(fd) currently is " << fd << ". Additional error information(errno): "
				<< strerror(errno) << ENDL;
			write(fd, time.c_str(), time.length());
			write(fd, "   ", 3);
			write(fd, output, temp);
			write(fd, "\n", 1);
			close(fd);
		}
		else if (mode == CONSOLE_OUTPUT)
			COUT << color << getCurrentTime() << output << DEFAULT << ENDL;
		va_end(args);
	}
}

/*This function is responsible for obtaining the current time, formatting it, and 
returning it as a string. The function obtains the current time, adjusts it for the 
time zone if necessary, and formats it into a string with a specific format, which 
includes the date and time. */
STR	Logger::getCurrentTime()
{
	tzset(); //This function is used to initialize time zone information. It ensures that the correct time zone is considered when formatting the time.
	char			date[1000];
	time_t			now = time(0);
	struct tm		tm = *gmtime(&now); //Converts the current time (in seconds since the epoch) into a tm struct representing a broken-down time in Coordinated Universal Time (UTC or GMT)
	tm.tm_hour =	tm.tm_hour + TIME_ZONE;
	strftime(date, sizeof(date), "[%Y-%m-%d  %H:%M:%S]      ", &tm); //Formats the time information from the tm struct into a human-readable string.
	return (STR(date));
}

void	Logger::setPrio(LogPrio prio)
{
	Logger::prio = prio;
}

void	Logger::setFileName(STR name)
{
	Logger::file_name = name;
}

void	Logger::setState(LogState state)
{
	Logger::current_state = state;
}
