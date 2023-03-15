#include "CommandManager.hpp"

CommandManager::CommandManager(): _commands() { };

CommandManager::CommandManager(const CommandManager &copy): _commands(copy._commands) { };

CommandManager::~CommandManager() { };

CommandManager  &CommandManager::operator=(const CommandManager &other) {
    this->_commands = other._commands;
}

void    CommandManager::registerCommand(std::string name, Command *command) {
    this->_commands.insert(std::make_pair(name, command));
}

void    CommandManager::registerCommands() {
	this->registerCommand("PASS", new PassCommand());
	this->registerCommand("USER", new UserCommand());
	this->registerCommand("NICK", new NickCommand());

	this->registerCommand("JOIN", new JoinCommand());
	this->registerCommand("PART", new PartCommand());
	this->registerCommand("TOPIC", new TopicCommand());
	this->registerCommand("NAMES", new NamesCommand());
	this->registerCommand("LIST", new ListCommand());
	this->registerCommand("MODE", new ModeCommand());
	this->registerCommand("KICK", new KickCommand());

	this->registerCommand("PRIVMSG", new PrivMsgCommand());
	this->registerCommand("NOTICE", new NoticeCommand());
};

Command *CommandManager::getCommand(std::string name) {
	return this->_commands[name];
} 