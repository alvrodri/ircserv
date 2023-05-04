#include "ModeCommand.hpp"
#include <regex>

ModeCommand::ModeCommand(): Command() {
}

ModeCommand::~ModeCommand() {
}

ModeCommand::ModeCommand(const ModeCommand &command) {
	this->operator=(command);
}

ModeCommand	&ModeCommand::operator=(const ModeCommand &command) {
	this->_args = command._args;
	return *this;
}

bool	ModeCommand::execute(Client &executor, std::vector<std::string> &args) const {
	Server	*server = executor.getServer();

	if (!executor.getAuthenticated()) {
		return true;
	}

	if (args.size() < 2) {
		server->reply(executor, "ERR_NEEDMOREPARAMS", args[0] + " :Not enough parameters");
		return true;
	}

	if (args.size() > 2 && !std::regex_match(args[2], std::regex("[\\+\\-opsitnbvw]+"))) {
		std::string	reply = "";
		reply.push_back(this->getUnknownMode(args[2]));

		server->reply(executor, "ERR_UNKNOWNMODE", reply + " :is unknown mode char to me");
		return true;
	}

	if (args[1].at(0) == '#' || args[1].at(0) == '&') {
		Channel *channel = channelExists(server, args[1]);

		if (!channel) {
			server->reply(executor, "ERR_NOSUCHCHANNEL", args[1] + " :No such channel");
			return true;
		}

		if (std::find(executor.getCurrentChannels().begin(), executor.getCurrentChannels().end(), channel) == executor.getCurrentChannels().end()) {
			server->reply(executor, "ERR_NOTONCHANNEL", args[1] + " :You're not on that channel");
			return true;
		}

		if (args.size() > 2 && !std::regex_match(args[2], std::regex("[\\+\\-ot]+"))) {
			server->reply(executor, "ERR_UMODEUNKNOWNFLAG", ":Unknown MODE flag");
			return true;
		}

		if (!channel->hasClientMode(&executor, 'o') && args.size() > 3) {
			server->reply(executor, "ERR_CHANOPRIVSNEEDED", channel->getName() + " :You're not channel operator");
			return true;
		}

		if (args.size() > 2) {
			bool add = false, changed = false;
			for (unsigned long i = 0; i < args[2].size(); i++) {
				if (args[2][i] == '+') {
					add = true;
					changed = true;
				} else if (args[2][i] == '-') {
					add = false;
					changed = true;
				} else if (args[2][i] == 't' && changed) {
					channel->setMode('t', add ? true : false);
				} else if (args[2][i] == 'o' && changed) {
					Client	*param = clientExists(server, args[3]);

					if (!param) {
						server->reply(executor, "ERR_NOSUCHNICK", args[3] + " :No such nick/channel");
						return true;
					}

					channel->setClientMode(param, 'o', add ? true : false);
				}
			}
		}

		server->reply(executor, "RPL_CHANNELMODEIS", channel->getName() + " +" + channel->getModes());

		return false;
	}

	if (args.size() > 2 && !std::regex_match(args[2], std::regex("[\\+\\-iwso]+"))) {
		server->reply(executor, "ERR_UMODEUNKNOWNFLAG", ":Unknown MODE flag");
		return true;
	}

	Client	*client = clientExists(server, args[1]);
	bool	add = false, changed = false;

	if (!client) {
		server->reply(executor, "ERR_NOSUCHNICK", args[1] + " :No such nick/channel");
		return true;
	}

	if (client->getNick() != executor.getNick()) {
		server->reply(executor, "ERR_USERSDONTMATCH", ":Cant change mode for other users");
		return true;
	}

	for (unsigned long i = 0; i < args[2].size(); i++) {
		if (args[2][i] == '+') {
			add = true;
			changed = true;
		} else if (args[2][i] == '-') {
			add = false;
			changed = true;
		} else {
			if (args[2][i] == 'o' && add && changed) {
				continue;
			}

			executor.setMode(args[2][i], (add && changed) ? true : false);
		}
	}

	server->reply(executor, "RPL_UMODEIS", ":" + (executor.getModes().empty() ? std::string("") : std::string("+")) + executor.getModes());

	return false;
}

char	ModeCommand::getUnknownMode(std::string &modes) const {
	std::string validModes = "+-opsitnbvw";

	for (unsigned long i = 0; i < modes.size(); i++) {
		unsigned long j = validModes.find(modes[i]);

		if (j == validModes.npos) {
			return modes[i];
		}
	}
	return '\0';
}
