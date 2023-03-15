#include "NamesCommand.hpp"

NamesCommand::NamesCommand(): Command() {
}

NamesCommand::~NamesCommand() {
}

NamesCommand::NamesCommand(const NamesCommand &command) {
	this->operator=(command);
}

NamesCommand	&NamesCommand::operator=(const NamesCommand &command) {
	this->_args = command._args;
	return *this;
}

bool	NamesCommand::execute(Client &executor, std::vector<std::string> &args) const {
	Server	*server = executor.getServer();

	if (!executor.getRegistered()) {
		return true;
	}
	
	if (args.size() <= 1) {
		for (std::map<std::string, Channel *>::iterator it = server->getChannels().begin(); it != server->getChannels().end(); it++) {
			server->sendNames(executor, *it->second);
		}
		return false;
	}

	std::vector<std::string>	channels = split(args[1], ',');
	for (int i = 0; i < channels.size(); i++) {
		Channel	*channel = channelExists(server, channels[i]);
		
		if (channel) {
			server->sendNames(executor, *channel);
		}
	}

	return false;
}