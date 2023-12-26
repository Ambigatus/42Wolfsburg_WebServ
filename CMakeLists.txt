cmake_minimum_required(VERSION 3.5)

project(WebServ)
set(CMAKE_BUILD_TYPE DEBUG)
set(SOURCE_EXE src/CGI/CGI_Configuration.cpp
src/CGI/CGI_Getters.cpp
src/CGI/CGI_Setters.cpp
src/Configuration/ConfigurationFile.cpp
src/Configuration/ConfigurationParser.cpp
src/Location/Location.cpp
src/Location/Getters.cpp
src/Location/Setters.cpp
src/Logger/Logger.cpp
src/Request/Request.cpp
src/Request/RequestParser.cpp
src/ServerConfiguration/Getters.cpp
src/ServerConfiguration/Setters.cpp
src/ServerConfiguration/Validation.cpp
src/ServerConfiguration/ServerConfiguration.cpp
src/Client.cpp
src/Errors_Handler.cpp
src/main.cpp
src/Mime.cpp
src/Response.cpp
src/ServerManager.cpp
src/Utility.cpp
        include/CGI_Configuration.hpp
include/Client.hpp
include/ConfigurationFile.hpp
include/ConfigurationParser.hpp
include/Location.hpp
include/Logger.hpp
include/Mime.hpp
include/Request.hpp
include/Response.hpp
include/ServerConfiguration.hpp
include/ServerManager.hpp
include/Utils.hpp
include/Webserv.hpp)

add_executable(webserver ${SOURCE_EXE})



