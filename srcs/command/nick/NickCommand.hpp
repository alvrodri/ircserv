#pragma once

#include "../Command.hpp"

class NickCommand : public Command {
public:
	NickCommand();
	~NickCommand();

	NickCommand(const NickCommand &command);

	NickCommand	&operator=(const NickCommand &command);

	bool	execute(Client &executor, std::vector<std::string> &args) const;
};