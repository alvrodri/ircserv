#pragma once

#include "../Command.hpp"

class PartCommand : public Command {
public:
	PartCommand();
	~PartCommand();

	PartCommand(const PartCommand &command);

	PartCommand	&operator=(const PartCommand &command);

	bool	execute(Client &executor, std::vector<std::string> &args) const;
};