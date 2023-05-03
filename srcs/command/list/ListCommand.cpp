#include "ListCommand.hpp"

ListCommand::ListCommand(): Command() {
}

ListCommand::~ListCommand() {
}

ListCommand::ListCommand(const ListCommand &command) {
	this->operator=(command);
}

ListCommand	&ListCommand::operator=(const ListCommand &command) {
	this->_args = command._args;
	return *this;
}

bool	ListCommand::execute(Client &executor, std::vector<std::string> &args) const {
	Server	*server = executor.getServer();

	if (!executor.getRegistered()) {
		return true;
	}

	server->reply(executor, "RPL_LISTSTART", "Channel :Users  Name");
	if (args.size() <= 1) {
		for (Server::t_channels::iterator it = server->getChannels().begin(); it != server->getChannels().end(); it++) {
			server->reply(executor, "RPL_LIST", it->second->getName() + " " + std::to_string(it->second->getClients().size()) + " :" + it->second->getTopic());
		}
	} else {
		std::vector<std::string> channels = split(args[1], ',');
		for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
			Channel *channel = channelExists(server, *it);

			if (channel) {
				server->reply(executor, "RPL_LIST", channel->getName() + " " + std::to_string(channel->getClients().size()) + " :" + channel->getTopic());
			}
		}
	}
	server->reply(executor, "RPL_LISTEND", ":End of /LIST");

	return true;
}