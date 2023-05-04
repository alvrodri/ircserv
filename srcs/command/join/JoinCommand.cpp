#include "JoinCommand.hpp"

JoinCommand::JoinCommand(): Command() {
}

JoinCommand::~JoinCommand() {
}

JoinCommand::JoinCommand(const JoinCommand &command) {
	this->operator=(command);
}

JoinCommand	&JoinCommand::operator=(const JoinCommand &command) {
	this->_args = command._args;
	return *this;
}

bool	JoinCommand::execute(Client &executor, std::vector<std::string> &args) const {
	Server	*server = executor.getServer();

	if (!executor.getRegistered()) {
		return true;
	}

	if (args.size() <= 1) {
		server->reply(executor, "ERR_NEEDMOREPARAMS", args[0] + " :Not enough parameters");
		return true;
	}

	std::vector<std::string>	toJoin = split(args[1], ',');
	for (unsigned long i = 0; i < toJoin.size(); i++) {
		int code = executor.joinChannel(toJoin[i]);
		Channel	*channel = server->getChannels()[toJoin[i]];

		if (code == 476) {
			server->reply(executor, "ERR_BADCHANMASK", "");
			break ;
		}

		if (code == 1)
			break ;

		if (code == -1) {
			server->reply(executor, channel->getTopic().size() == 0 ? "RPL_NOTOPIC" : "RPL_TOPIC",
				channel->getName() + " :" + (channel->getTopic().size() == 0 ? "No topic is set" : channel->getTopic()));
			
			std::string	clients = "";
			for (unsigned long i = 0; i < channel->getClients().size(); i++) {
				clients += channel->hasClientMode(channel->getClients()[i], 'o') ? "@" : "";
				clients += channel->getClients()[i]->getNick();
				clients += " ";
			}
			clients.pop_back();
	
			server->simpleReply(channel->getClients(), ":" + executor.getNick() + " JOIN :" + channel->getName());
			server->reply(executor, "RPL_NAMREPLY", "= " +  channel->getName() + " :" + clients);
			server->reply(executor, "RPL_ENDOFNAMES", channel->getName() + " :End of /NAMES list");
		}
	}

	return true;
}
