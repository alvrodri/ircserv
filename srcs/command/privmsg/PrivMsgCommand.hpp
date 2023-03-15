#pragma once

#include "../Command.hpp"

class PrivMsgCommand : public Command {
public:
	PrivMsgCommand();
	~PrivMsgCommand();

	PrivMsgCommand(const PrivMsgCommand &command);

	PrivMsgCommand	&operator=(const PrivMsgCommand &command);

	bool	execute(Client &executor, std::vector<std::string> &args) const;
};