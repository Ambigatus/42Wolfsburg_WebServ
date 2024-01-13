#ifndef UTILS_HPP
# define UTILS_HPP

# include "Webserv.hpp"

/* Colors*/
#define RESET          "\x1B[0m"
#define RED            "\x1B[31m"
#define LIGHT_RED      "\x1B[91m"
#define WHITE          "\x1B[37m"
#define BLINK           "\x1b[5m"
#define YELLOW         "\x1B[33m"
#define LIGHT_BLUE     "\x1B[94m"
#define CYAN           "\x1B[36m"
#define DARK_GREY      "\x1B[90m"
#define LIGHTMAGENTA   "\x1B[95m"
#define GST             4;



/* Macros */

# define COUT		std::cout //cout
# define CERR		std::cerr //cerr
# define ENDL		std::endl //endl
# define STR		std::string //string
# define S_STREAM	std::stringstream //stringstream
# define VECTOR		std::vector //vector container
# define EXCEPTION	std::exception() //exception
# define HEX		std::hex // hexadecimal output mode
# define MAP		std::map //map container
# define LINE	"==****************************************************************=="

/* Other */

# define CONNECTION_TIMEOUT 60	//Standart time for disconnect client from server, if no data was sent
# define MAX_URI_LENGTH 4096 //This constant used to represent the maximum allowed length for a Uniform Resource Identifier (URI) string.
# define MAX_CONTENT_LENGTH 30000000 //This constant used to specify the maximum allowed size (in bytes) for the content or payload of an HTTP request.
# define MESSAGE_BUFFER 40000

template <typename T>
STR toString(const T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

STR statusCodeString(short);
STR getErrorPage(short);
int buildHtmlIndex(STR &, VECTOR<uint8_t> &, size_t &);
int strToInt(STR str);
unsigned int fromHexToDec(const STR& nb);

#endif
