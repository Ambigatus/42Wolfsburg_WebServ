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

/*This function creates an HTML directory listing for a specified directory, including file 
and directory names, last modification times, and file sizes. The generated HTML content 
is stored in the body vector, and its length is provided in length. */
int createHTMLIndex(STR &dir_name, VECTOR<uint8_t> &body, size_t length)
{
	struct dirent	*wholeStruct;
	DIR				*directory;
	STR				listPage;

	directory = opendir(dir_name.c_str()); //we start from opening specified directory. 
	if (directory == NULL)
	{
		CERR << B_RED << "Can't open this directory(opendir failed)." << DEFAULT << ENDL;
		return 1;
	}
	listPage.append("<html>\n"); //here we creating the HTML structure for the directory listing
    listPage.append("<head>\n");
    listPage.append("<title> Index of");
    listPage.append(dir_name);
    listPage.append("</title>\n");
    listPage.append("</head>\n");
    listPage.append("<body >\n");
    listPage.append("<h1> Index of " + dir_name + "</h1>\n");
    listPage.append("<table style=\"width:80%; font-size: 15px\">\n");
    listPage.append("<hr>\n");
    listPage.append("<th style=\"text-align:left\"> File Name </th>\n");
    listPage.append("<th style=\"text-align:left\"> Last Modification  </th>\n");
    listPage.append("<th style=\"text-align:left\"> File Size </th>\n");

	struct stat fileStat; //struct stat that hold information about a file, such as its size, permissions, and timestamps.
	STR			filePath;

	while ((wholeStruct == readdir(directory)) != NULL) //going into whole directory
	{
		if (strcmp(wholeStruct->d_name, ".") == 0) //checks entry name
			continue;
		filePath = dir_name + wholeStruct->d_name; //constracting full path
		stat(filePath.c_str(), &fileStat); //retrieves file stat information
		listPage.append("<tr>\n"); //add information about the file or directory to HTML table
        listPage.append("<td>\n");
        listPage.append("<a href=\"");
        listPage.append(wholeStruct->d_name);
		if (S_ISDIR(fileStat.st_mode))
			listPage.append("/");
		listPage.append("\">");
		listPage.append(wholeStruct->d_name);
        if (S_ISDIR(fileStat.st_mode))
            listPage.append("/");
        listPage.append("</a>\n");
        listPage.append("</td>\n");
        listPage.append("<td>\n");
        listPage.append(ctime(&fileStat.st_mtime));
        listPage.append("</td>\n");
        listPage.append("<td>\n");
        if (!S_ISDIR(fileStat.st_mode))
            listPage.append(toString(fileStat.st_size));
        listPage.append("</td>\n");
        listPage.append("</tr>\n");
	}
	listPage.append("</table>\n");
    listPage.append("<hr>\n");

    listPage.append("</body>\n");
    listPage.append("</html>\n");

    body.insert(body.begin(), listPage.begin(), listPage.end()); //insert generated content into "body"
    length = body.size(); // calculates lenght of the HTML content
    return (0);
}


