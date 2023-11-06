#include "../include/ConfigurationParser.hpp"
#include "../include/ConfigurationFile.hpp"
#include "../include/Request.hpp"

// Cnfiguration parser test

// int main() {

// 	std::string configFilePath = "../conf/default.conf";
// 	ConfigurationFile configFile(configFilePath);
// 	return 0;
// 	int fileCheckResult = configFile.checkConfigFile(configFilePath, R_OK);
// 	if (fileCheckResult != 0) {
// 		std::cerr << "Error: Configuration file is not accessible or does not exist." << std::endl;
// 		return 1;
// 	}

// 	std::string fileContent = configFile.readFile(configFilePath);
// 	ConfigurationParser configParser;
// 	int parseResult = configParser.parseConfig(fileContent);
// 	if (parseResult != 0) {
// 		std::cerr << "Error: Failed to parse the configuration file." << std::endl;
// 		return 1;
// 	}
// 	configParser.printConfig();
// 	return 0;
// }


// Request test

int main()
{
	char requestData[] = "GET /index.html HTTP/1.1\r\n"
						"Host: www.example.com\r\n"
						"User-Agent: MyUserAgent\r\n"
						"Accept: text/html\r\n"
						"\r\n" 
						"This is the HTTP request body.";

	size_t requestDataSize = sizeof(requestData) - 1;

	Request httpRequest;

	httpRequest.feed(requestData, requestDataSize);


	COUT << "HTTP Method: " << httpRequest.getMethodStr() << ENDL;
	COUT << "Path: " << httpRequest.getPath() << ENDL;

	COUT << "Request Body: " << httpRequest.getBody() << ENDL;
	httpRequest.printMessage();
	return 0;
}

