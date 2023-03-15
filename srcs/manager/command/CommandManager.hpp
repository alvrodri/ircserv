#pragma once

#include "../command/Command.hpp"
#include "../command/nick/NickCommand.hpp"
#include "../command/user/UserCommand.hpp"
#include "../command/pass/PassCommand.hpp"
#include "../command/join/JoinCommand.hpp"
#include "../command/privmsg/PrivMsgCommand.hpp"
#include "../command/notice/NoticeCommand.hpp"
#include "../command/part/PartCommand.hpp"
#include "../command/names/NamesCommand.hpp"
#include "../command/topic/TopicCommand.hpp"
#include "../command/list/ListCommand.hpp"
#include "../command/mode/ModeCommand.hpp"
#include "../command/kick/KickCommand.hpp"

#include <map>

class CommandManager {
    public:
    typedef std::map<std::string, Command *> commandMap;

    private:
    commandMap 	_commands;

    public:
    CommandManager();
    CommandManager(const CommandManager &copy);

    ~CommandManager();

    CommandManager  &operator=(const CommandManager &other);

    void    registerCommand(std::string, Command *);
    void    registerCommands();

    Command *getCommand(std::string name);
};