#include "PartCommand.hpp"

PartCommand::PartCommand(): Command() {
}

PartCommand::~PartCommand() {
}

PartCommand::PartCommand(const PartCommand &command) {
	this->operator=(command);
}

PartCommand	&PartCommand::operator=(const PartCommand &command) {
	this->_args = command._args;
	return *this;
}

/*
	4.2.2 Part message

	Command: PART
	Parameters: <channel>{,<channel>}

	The PART message causes the client sending the message to be removed
	from the list of active users for all given channels listed in the
	parameter string.

	Numeric Replies:
		ERR_NOSUCHCHANNEL	ERR_NOTONCHANNEL
*/

bool	PartCommand::execute(Client &executor, std::vector<std::string> &args) const {
	Server	*server = executor.getServer();

	if (!executor.getRegistered()) {
		return true;
	}

	if (args.size() <= 1) {
		server->reply(executor, "ERR_NEEDMOREPARAMS", args[0] + " :Not enough parameters");
		return true;
	}

	std::vector<std::string>	toLeave = split(args[1], ',');
	for (unsigned long i = 0; i < toLeave.size(); i++) {
		Channel *channel = channelExists(server, toLeave[i]);

		if (!channel) {
			server->reply(executor, "ERR_NOSUCHCHANNEL", toLeave[i] + " :No such channel");
			break;
		}

		if (std::find(executor.getCurrentChannels().begin(), executor.getCurrentChannels().end(), channel) == executor.getCurrentChannels().end()) {
			server->reply(executor, "ERR_NOTONCHANNEL", toLeave[i] + " :You're not on that channel");
			break;
		}

		executor.leaveChannel(toLeave[i]);
	}

	return true;
}
