NAME = webserv

SRC =		src/main.cpp \
			src/Utility.cpp \
			src/ServerManager.cpp \
			src/Response.cpp \
			src/Client.cpp \
			src/RequestParser.cpp \
			src/Request.cpp \
			src/Configuration/ConfigurationFile.cpp \
			src/Configuration/ConfigurationParser.cpp \
			src/ServerConfiguration/Getters.cpp \
			src/ServerConfiguration/Setters.cpp \
			src/ServerConfiguration/Validation.cpp \
			src/ServerConfiguration/ServerConfiguration.cpp \
			src/Location/Getters.cpp \
			src/Location/Setters.cpp \
			src/Location/Location.cpp \
			src/CGI/CGI_Getters.cpp \
			src/CGI/CGI_Setters.cpp \
			src/CGI/CGI_Configuration.cpp \
			src/Errors_Handler.cpp \
			src/Logger/Logger.cpp \
			src/Mime.cpp \
		
HEADER =	include/Webserv.hpp \
		
OBJ = $(SRC:.cpp=.o)

CXX = c++

CXXFLAGS = -Wextra -Wall -Werror -std=c++98 -pedantic #-std=c++11 

RM = rm -rf

GRN	=	"\033[32m"
END	=	"\033[00m"

all:		$(NAME)

$(NAME):	$(OBJ) $(HEADER)
			$(CXX) $(CXXFLAGS) $(OBJ) -I. -o $(NAME)
			@echo $(GRN) "\tCompiled "$(NAME)" successfully!" $(END)

clean:
			$(RM) $(OBJ)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re 