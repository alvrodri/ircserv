#pragma once

#include "../Command.hpp"

class ListCommand : public Command {
public:
	ListCommand();
	~ListCommand();

	ListCommand(const ListCommand &command);

	ListCommand	&operator=(const ListCommand &command);

	bool	execute(Client &executor, std::vector<std::string> &args) const;
};