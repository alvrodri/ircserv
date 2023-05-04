#include "PrivMsgCommand.hpp"

PrivMsgCommand::PrivMsgCommand(): Command() {
}

PrivMsgCommand::~PrivMsgCommand() {
}

PrivMsgCommand::PrivMsgCommand(const PrivMsgCommand &command) {
	this->operator=(command);
}

PrivMsgCommand	&PrivMsgCommand::operator=(const PrivMsgCommand &command) {
	this->_args = command._args;
	return *this;
}

bool	PrivMsgCommand::execute(Client &executor, std::vector<std::string> &args) const {
	Server	*server = executor.getServer();

	if (!executor.getRegistered()) {
		return true;
	}

	if (args.size() <= 1) {
		server->reply(executor, "ERR_NORECIPIENT", ":No recipient given (" + args[0] + ")");
		return true;
	}

	if (args.size() <= 2) {
		server->reply(executor, "ERR_NOTEXTTOSEND", ":No text to send");
		return true;
	}

	std::string text = "";
	for (unsigned long i = 2; i < args.size(); i++) {
		text += args[i];

		if (args[2][0] != ':')
			break;
		
		if (i != args.size() - 1)
			text += " ";
	}

	std::vector<std::string> recipents = split(args[1], ',');
	for (unsigned long i = 0; i < recipents.size(); i++) {
		if (std::count(recipents.begin(), recipents.end(), recipents[i]) > 1) {
			server->reply(executor, "ERR_TOOMANYTARGETS", recipents[i] + " :Duplicate recipients. No message delivered");
			return true;
		}

		if (recipents[i].at(0) != '#' && recipents[i].at(0) != '&') {
			Client	*receiver = clientExists(server, recipents[i]);

			if (!receiver) {
				server->reply(executor, "ERR_NOSUCHNICK", recipents[i] + " :No such nick/channel");
				return true;
			} else {
				std::string message = ":" + executor.getNick() + " PRIVMSG " + receiver->getNick() + " " + text;
				server->sendMessage(*receiver, message);
			}

			return false;
		}

		if (recipents[i].at(0) == '#' || recipents[i].at(0) == '&') {
			Channel	*channel = channelExists(server, recipents[i]);

			if (!channel) {
				server->reply(executor, "ERR_NOSUCHNICK", recipents[i] + " :No such nick/channel");
				return true;
			} else {
				std::string message = ":" + executor.getNick() + " PRIVMSG " + channel->getName() + " " + text;
				server->sendMessage(executor, *channel, message);
			}
		}
	}

	
	return false;
}
