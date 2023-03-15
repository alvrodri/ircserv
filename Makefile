NAME = ft_irc

SRCS = ./srcs/main.cpp ./srcs/channel/Channel.cpp ./srcs/command/Command.cpp ./srcs/server/Server.cpp ./srcs/client/Client.cpp ./srcs/command/user/UserCommand.cpp ./srcs/command/mode/ModeCommand.cpp ./srcs/command/part/PartCommand.cpp ./srcs/command/notice/NoticeCommand.cpp ./srcs/command/list/ListCommand.cpp ./srcs/command/privmsg/PrivMsgCommand.cpp ./srcs/command/nick/NickCommand.cpp ./srcs/command/pass/PassCommand.cpp ./srcs/command/kick/KickCommand.cpp ./srcs/command/names/NamesCommand.cpp ./srcs/command/join/JoinCommand.cpp ./srcs/command/topic/TopicCommand.cpp ./srcs/utils/Utils.cpp
OBJS = $(SRCS:.cpp=.o)

CXX = clang++
CXXFLAGS = -fsanitize=address -g3

$(NAME):        $(OBJS)
		$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

all:    $(NAME)

clean:
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
