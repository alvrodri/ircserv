#pragma once

#include "../Command.hpp"

class JoinCommand : public Command {
public:
	JoinCommand();
	~JoinCommand();

	JoinCommand(const JoinCommand &command);

	JoinCommand	&operator=(const JoinCommand &command);

	bool	execute(Client &executor, std::vector<std::string> &args) const;
};