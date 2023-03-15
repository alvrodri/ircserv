#include "PassCommand.hpp"

PassCommand::PassCommand(): Command() {
}

PassCommand::~PassCommand() {
}

PassCommand::PassCommand(const PassCommand &command) {
	this->operator=(command);
}

PassCommand	&PassCommand::operator=(const PassCommand &command) {
	this->_args = command._args;
	return *this;
}

bool	PassCommand::execute(Client &executor, std::vector<std::string> &args) const {
	Server	*server = executor.getServer();

	if (args.size() != 2) {
		server->reply(executor, "ERR_NEEDMOREPARAMS", ":Not enough parameters");
		return false;
	}

	if (executor.getRegistered()) {
		server->reply(executor, "ERR_ALREADYREGISTRED", ":Unauthorized command (already registered)");
		return false;
	}

	executor.setAuthenticated();

	return true;
}