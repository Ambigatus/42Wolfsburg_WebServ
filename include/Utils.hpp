#ifndef UTILS_HPP
# define UTILS_HPP

//# include <iostream>
//# include <string>
//# include <sstream>
//# include <vector>
//# include <stack>

/* Colors*/
# define DEFAULT "\033[0;39m"
# define B_RED "\033[1;31m"
# define B_BLUE "\033[1;94m"
# define B_YELLOW "\033[1;33m"
# define B_GREEN "\033[1;32m"
# define B_PINK "\033[1;35m"
# define B_TURQUOISE "\033[36;1m"
# define U_RED	"\e[4;31m"
# define U_BLUE	"\e[4;34m"
// # define U_YELLOW	"\e[4;33m"
# define U_GREEN	"\e[4;32m"
# define U_PINK	"\e[4;35m"
# define U_TURQUOSIE	" \e[4;36m"

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

/* Templates */

/*This template converts any datatype T into string */
template <typename T>
STR toString(const T val)
{
	S_STREAM stream;
	stream << val; //inserts value into stringstream
	return stream.str(); //converted data
}

/* Utility functions */

STR statusCodeList(short);
STR showErrorPage(short);
int strToInt(STR str);
unsigned int hexToDec(const STR& num);

#endif
