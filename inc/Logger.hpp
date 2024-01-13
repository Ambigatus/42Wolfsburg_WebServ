#ifndef LOGGER_HPP
#define LOGGER_HPP

# include "Webserv.hpp"

enum LogPrio{
    DEBUG,
    INFO,
    ERROR
};

enum L_State{
    ON,
    OFF
};


enum Mode{
    CONSOLE_OUTPUT,
    FILE_OUTPUT
};

class Logger{

    public:
        static STR                          file_name;
        static LogPrio                      prio;
        static MAP<LogPrio, STR>            prio_str;
        static L_State state;

        static void                         setFilenName(STR);
        static void                         setState(L_State);

        static void                         logMsg(const char *, Mode, const char*, ...);
        static void                         setPrio(LogPrio);
        static STR                          getCurrTime();

    private:
        static MAP<LogPrio, STR> initMap();

};

#endif
