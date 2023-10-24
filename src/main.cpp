#include "../include/ConfigurationParser.hpp"
#include "../include/ConfigurationFile.hpp"

// Cnfiguration parser test

int main() {

	std::string configFilePath = "../conf/default.conf";
	ConfigurationFile configFile(configFilePath);
	return 0;
	int fileCheckResult = configFile.checkConfigFile(configFilePath, R_OK);
	if (fileCheckResult != 0) {
		std::cerr << "Error: Configuration file is not accessible or does not exist." << std::endl;
		return 1;
	}

	std::string fileContent = configFile.readFile(configFilePath);
	ConfigurationParser configParser;
	int parseResult = configParser.parseConfig(fileContent);
	if (parseResult != 0) {
		std::cerr << "Error: Failed to parse the configuration file." << std::endl;
		return 1;
	}
	configParser.printConfig();
	return 0;
}
