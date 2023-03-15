#pragma once

#include "../Command.hpp"

class PassCommand : public Command {
public:
	PassCommand();
	~PassCommand();

	PassCommand(const PassCommand &command);

	PassCommand	&operator=(const PassCommand &command);

	bool	execute(Client &executor, std::vector<std::string> &args) const;
};