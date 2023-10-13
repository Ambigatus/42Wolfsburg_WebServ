#include "../include/Webserv.hpp"

/*The function ensures that the input string is a valid integer, converts it to an int, and returns the result. 
If the string is too long or contains non-numeric characters, it throws exceptions to indicate the issues.*/
int strToInt(STR str)
{
	S_STREAM ss(str);
	if (str.length() > 10)
		throw EXCEPTION;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if(!isdigit(str[i]))
			throw EXCEPTION;
	}
	int	result;
	ss >> result;
	return result;
}

/*The hexToDec function takes a hexadecimal string and converts it to an unsigned decimal integer.
The function returns the decimal result.*/
unsigned int hexToDec(const STR& num)
{
	unsigned int	result;
	S_STREAM	ss;
	ss << num;
	ss >> HEX >> result;
	return result;
}

