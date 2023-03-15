#include "TopicCommand.hpp"

TopicCommand::TopicCommand(): Command() {
}

TopicCommand::~TopicCommand() {
}

TopicCommand::TopicCommand(const TopicCommand &command) {
	this->operator=(command);
}

TopicCommand	&TopicCommand::operator=(const TopicCommand &command) {
	this->_args = command._args;
	return *this;
}

bool	TopicCommand::execute(Client &executor, std::vector<std::string> &args) const {
	Server	*server = executor.getServer();

	if (!executor.getRegistered()) {
		return true;
	}

	if (args.size() < 2) {
		server->reply(executor, "ERR_NEEDMOREPARAMS", args[0] + " :Not enough parameters");
		return true;
	}

	Channel *channel = channelExists(server, args[1]);
	if (!channel) {
		return true;
	}

	if (std::find(executor.getCurrentChannels().begin(), executor.getCurrentChannels().end(), channel) == executor.getCurrentChannels().end()) {
		server->reply(executor, "ERR_NOTONCHANNEL", args[1] + " :You're not on that channel");
		return true;
	}

	if (args.size() == 2) {
		if (channel->getTopic().empty()) {
			server->reply(executor, "RPL_NOTOPIC", args[1] + " :No topic is set");
		} else {
			server->reply(executor, "RPL_TOPIC", args[1] + " :" + channel->getTopic());
		}
		return false;
	}

	if (channel->hasMode('t') && !channel->hasClientMode(&executor, 'o')) {
		server->reply(executor, "ERR_CHANOPRIVSNEEDED", channel->getName() + " :You're not channel operator");
		return true;
	}

	channel->setTopic(getCompleteArgument(args, 2));
	server->reply(channel->getClients(), "RPL_TOPIC", args[1] + " :" + channel->getTopic());

	return false;
}