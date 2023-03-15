#pragma once

#include "../Command.hpp"

class UserCommand : public Command {
public:
	UserCommand();
	~UserCommand();

	UserCommand(const UserCommand &command);

	UserCommand	&operator=(const UserCommand &command);

	bool	execute(Client &executor, std::vector<std::string> &args) const;
};