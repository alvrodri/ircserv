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

	for (std::map<int, Client>::iterator it = server->getClients().begin(); it != server->getClients().end(); it++) {
		if ((*it).second.getNick() == args[1]) {
			server->reply(executor, "ERR_NICKNAMEINUSE", args[1] +  " :Nickname is already in use");
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
