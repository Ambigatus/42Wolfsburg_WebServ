NAME = webserv

SRCS = src/main.cpp src/Utils.cpp src/ServerManager.cpp src/Response.cpp src/Client.cpp src/Request.cpp \
	   src/ConfigurationFile.cpp src/ConfigParser.cpp src/ServerConfig.cpp src/Location.cpp src/CGIConfig.cpp \
	   src/Mime.cpp src/Logger.cpp

HEADERS	= inc/Webserv.hpp

OBJS = $(SRCS:.cpp=.o)

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98
CXXFLAGS += -g3 

RM = rm -rf

RESET = "\033[0m"
BLACK = "\033[1m\033[37m"

all:
	@$(MAKE) $(NAME) -j5
$(NAME) : $(OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo $(BLACK) Amazing WebServ is ready to work🤖 $(RESET)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: 	fclean all

.PHONY: all clean fclean re