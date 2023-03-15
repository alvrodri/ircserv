#pragma once

#include "../Command.hpp"

class NamesCommand : public Command {
public:
	NamesCommand();
	~NamesCommand();

	NamesCommand(const NamesCommand &command);

	NamesCommand	&operator=(const NamesCommand &command);

	bool	execute(Client &executor, std::vector<std::string> &args) const;
};