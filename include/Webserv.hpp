#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/*						Libraries section						*/
/*						Base libraries for whole work			*/
# include <unistd.h>
# include <fcntl.h> //The <fcntl.h> library in C and C++ provides functions and constants related to file control. 
# include <dirent.h> //The dirent.h library is a C and C++ library that provides functions and data structures for working with directory entries.
# include <iostream> //The <iostream> library in C++ is a standard library that provides input and output functionality.
# include <fstream> //The <fstream> library in C++ is a standard library that provides facilities for working with file input and output. 
# include <sstream> //The <sstream> library in C++ is a standard library that provides facilities for working with string streams. 
# include <string>
# include <cstring> //The <cstring> library in C++ provides functions for manipulating C-style strings, which are null-terminated character arrays.
# include <cstdlib> //The <cstdlib> library in C++ provides functions for general-purpose operations and conversions.
# include <cctype> //The <cctype> library in C++ provides functions for character handling.
# include <ctime>
# include <cstdarg> //The <cstdarg> library in C++ provides functions for handling variable arguments in functions.

/*						Containers								*/
# include <vector>
# include <map>
# include <set>
# include <algorithm>
# include <iterator>
# include <list>

/*						Networking								*/
# include <netinet/in.h>
# include <arpa/inet.h>

/*						System control							*/
# include <sys/wait.h> //The sys/wait.h library in C and C++ provides functions and macros for working with process management and handling child processes.
# include <sys/types.h> //The <sys/types.h> library in C and C++ provides a set of fundamental data types and structures that are used for system-level programming.
# include <sys/stat.h> //The <sys/stat.h> library in C and C++ provides functions and data structures for working with file and file system-related information.
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/time.h> //The <sys/time.h> library in C and C++ provides functionality for working with time and timing-related operations.
# include <signal.h> //The <signal.h> library in C and C++ provides features for working with signals, which are software interrupts delivered to a running process.


/*						HPP Files								*/
# include "ConfigurationFile.hpp"
# include "ConfigurationParser.hpp"
# include "Logger.hpp"
# include "ServerConfiguration.hpp"
# include "Utils.hpp"

#endif