#include "KickCommand.hpp"

KickCommand::KickCommand(): Command() {
}

KickCommand::~KickCommand() {
}

KickCommand::KickCommand(const KickCommand &command) {
	this->operator=(command);
}

KickCommand	&KickCommand::operator=(const KickCommand &command) {
	this->_args = command._args;
	return *this;
}

bool	KickCommand::execute(Client &executor, std::vector<std::string> &args) const {
	Server	*server = executor.getServer();

	if (!executor.getRegistered()) {
		return true;
	}

	if (args.size() < 4) {
		server->reply(executor, "ERR_NEEDMOREPARAMS", args[0] + " :Not enough parameters");
		return true;
	}

	Channel	*channel = channelExists(server, args[1]);
	if (!channel) {
		server->reply(executor, "ERR_NOSUCHCHANNEL", args[1] + " :No such channel");
		return true;
	}

	if (std::find(executor.getCurrentChannels().begin(), executor.getCurrentChannels().end(), channel) == executor.getCurrentChannels().end()) {
		server->reply(executor, "ERR_NOTONCHANNEL", args[1] + " :You're not on that channel");
		return true;
	}

	Client	*kicked = clientExists(server, args[2]);
	if (!kicked || std::find(kicked->getCurrentChannels().begin(), kicked->getCurrentChannels().end(), channel) == kicked->getCurrentChannels().end()) {
		server->reply(executor, "ERR_USERNOTINCHANNEL", args[2] + " " + channel->getName() + " :They aren't on that channel");
		return true;
	}

	if (!channel->hasClientMode(&executor, 'o')) {
		server->reply(executor, "ERR_CHANOPRIVSNEEDED", args[2] + " " + channel->getName() + " :They aren't on that channel");
		return true;
	}

	std::string reason = "";
	if (args.size() > 3) {
		reason = getCompleteArgument(args, 3);
	}
	server->simpleReply(channel->getClients(), ":" + executor.getNick() + " KICK " + channel->getName() + " " + kicked->getNick() + (args.size() > 3 ? " :" + reason : ""));
	kicked->leaveChannel(channel->getName(), true);

	return false;
}