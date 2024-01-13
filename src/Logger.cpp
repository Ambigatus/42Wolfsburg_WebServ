#include "../inc/Logger.hpp"


STR Logger::file_name = "logfile.txt";
LogPrio Logger::prio = ERROR;
L_State Logger::state = ON;

MAP<LogPrio, STR> Logger::prio_str = initMap();


MAP<LogPrio, STR> Logger::initMap()
{
    std::map<LogPrio, STR> p_map;

    p_map[DEBUG] = "[INFO]    ";
    p_map[INFO] = "[DEBUG]    ";
    p_map[ERROR] = "[ERROR]   ";
    return p_map;
}

void    Logger::logMsg(const char *color, Mode m, const char* msg, ...)
{
    char        output[8192];
    va_list     args;
    int         n;

    if (state == ON)
    {
        va_start(args, msg);
        n = vsnprintf(output, 8192, msg, args);
        STR date = getCurrTime();
        if (m == FILE_OUTPUT)
        {
            if (mkdir("logs", 0777) < 0 && errno != EEXIST)
            {
                std::cerr << "mkdir() Error: " << strerror(errno) << ENDL;
                return ;
            }
            int fd = open(("logs/" + file_name).c_str(), O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR);
            COUT << "fd is " << fd << "And errno is :" << strerror(errno) << ENDL;
            write(fd, date.c_str(), date.length());
            write(fd, "   ", 3);
            write(fd, output, n);
            write(fd, "\n", 1);
            close(fd);
        }
        else if (m == CONSOLE_OUTPUT)
            COUT << color << getCurrTime() << output << RESET << ENDL;
        va_end(args);
    }
}

STR Logger::getCurrTime()
{
    tzset();
    char date[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    tm.tm_hour = tm.tm_hour + GST;
    strftime(date, sizeof(date), "[%Y-%m-%d  %H:%M:%S]   ", &tm);
    return (STR(date));
}

void Logger::setPrio(LogPrio p)
{
    Logger::prio = p;
}

void Logger::setFilenName(STR name)
{
    Logger::file_name = name;
}

void Logger::setState(L_State s)
{
    Logger::state = s;
}


