#include "UserCommand.hpp"

UserCommand::UserCommand(): Command() {
}

UserCommand::~UserCommand() {
}

UserCommand::UserCommand(const UserCommand &command) {
	this->operator=(command);
}

UserCommand	&UserCommand::operator=(const UserCommand &command) {
	this->_args = command._args;
	return *this;
}

bool	UserCommand::execute(Client &executor, std::vector<std::string> &args) const {
	if (!executor.getAuthenticated()) {
		return false;
	}

	Server	*server = executor.getServer();

	if (args.size() < 5) {
		server->reply(executor, "ERR_NEEDMOREPARAMS", "USER :Not enough parameters");
		return false;
	}

	if (executor.getRegistered()) {
		server->reply(executor, "ERR_ALREADYREGISTRED", ":You may not reregister");
		return false;
	}

	executor.setUsername(args[1]);
	executor.setHostname(args[2]);
	executor.setServername(args[3]);

	std::string realName = "";
	for (unsigned long i = 4; i < args.size(); i++) {
		realName += args[i];
		if (i != args.size() - 1)
			realName += " ";
	}

	if (realName[0] == ':') {
		realName.erase(realName.begin());
	}

	executor.setRealname(realName);

	if (!executor.getNick().empty() && !executor.getUsername().empty() && !executor.getRealname().empty() && !executor.getServername().empty() && !executor.getHostname().empty()) {
		executor.setRegistered();
	}
	return true;
}
