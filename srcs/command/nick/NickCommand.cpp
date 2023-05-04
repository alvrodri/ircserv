#include "NickCommand.hpp"
#include <regex>
#include <string>

NickCommand::NickCommand(): Command() {
}

NickCommand::~NickCommand() {
}

NickCommand::NickCommand(const NickCommand &command) {
	this->operator=(command);
}

NickCommand	&NickCommand::operator=(const NickCommand &command) {
	this->_args = command._args;
	return *this;
}

bool	NickCommand::execute(Client &executor, std::vector<std::string> &args) const {
	Server	*server = executor.getServer();

	if (!executor.getAuthenticated()) {
		return false;
	}

	if (args.size() < 2) {
		server->reply(executor, "ERR_NONICKNAMEGIVEN",  ":No nickname given");
		return false;
	}

  std::string givenNick = args.at(1);
  for (std::string::iterator it = givenNick.begin(); it != givenNick.end(); it++)
    *it = std::tolower(*it);

	for (std::map<int, Client>::iterator it = server->getClients().begin(); it != server->getClients().end(); it++) {
		std::string currentNick = (*it).second.getNick();
		for (std::string::iterator it = currentNick.begin(); it != currentNick.end(); it++)
		  *it = std::tolower(*it);
		if (currentNick == givenNick) {
			if (executor.getRegistered()) {
				server->reply(executor, "ERR_NICKNAMEINUSE", args[1] +  " :Nickname is already in use");
			} else {
				server->reply(executor, "ERR_NICKCOLLISION", args[1] +  " :Nickname collision KILL");
			}
			return false;
		}
	}

	if (!std::regex_match(args[1], std::regex("^[a-zA-Z][a-zA-Z0-9-\\[\\]\\\\`^{}]*$"))) {
		server->reply(executor, "ERR_ERRONEUSNICKNAME", args[1] + " :Erroneus nickname");
		return false;
	}

	if (!executor.getNick().empty()) {
		for (std::map<int, Client>::iterator it = server->getClients().begin(); it != server->getClients().end(); it++) {
			executor.getServer()->simpleReply(it->second, ":" + executor.getNick() + " NICK " + args[1]);
		}
	}
	executor.setNick(args[1]);

	return true;
}
