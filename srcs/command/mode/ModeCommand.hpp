#pragma once

#include "../Command.hpp"

class ModeCommand : public Command {
	public:
	ModeCommand();
	~ModeCommand();

	ModeCommand(const ModeCommand &command);

	ModeCommand	&operator=(const ModeCommand &command);

	bool	execute(Client &executor, std::vector<std::string> &args) const;

	private:
	char	getUnknownMode(std::string &modes) const;
};