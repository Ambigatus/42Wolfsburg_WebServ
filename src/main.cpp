#include "../include/Webserv.hpp"

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

// int main()
// {
// 	char requestData[] = "GET /index.html HTTP/1.1\r\n"
// 						"host: www.example.com\r\n"
// 						"user-Agent: MyUserAgent\r\n"
// 						"accept: text/html\r\n"
// 						"content-length: 6372\r\n"
// 						"content-type: text/html\r\n"
// 						"\r\n"
// 						"This is the HTTP request body.";

// 	size_t requestDataSize = sizeof(requestData) - 1;

// 	Request httpRequest;

// 	httpRequest.feed(requestData, requestDataSize);


// 	COUT << "HTTP Method: " << httpRequest.getMethodStr() << ENDL;
// 	COUT << "Path: " << httpRequest.getPath() << ENDL;

// 	COUT << "Request Body: " << httpRequest.getBody() << ENDL;
// 	httpRequest.printMessage();
// 	return 0;
// }


int main() {
	// Create a sample HttpRequest (you might need to adjust it based on your implementation)
	Request request;
	request.setMethod(GET);
	request.setPath("/index.html");
	request.setHeader("accept", "text/html");
	// Add other headers as needed

	// Create a sample ServerConfig (you might need to adjust it based on your implementation)
	ServerConfiguration server;
	// Initialize server configuration as needed

	// Create a Response object
	Response response(request);

	// Set the server configuration
	response.setServer(server);

	// Build the response
	response.buildResponse();

	// Print the generated response
	std::cout << response.getResponse() << std::endl;

	return 0;
}


