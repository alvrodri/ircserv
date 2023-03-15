#pragma once

#include <iostream>
#include <vector>

#include "../client/Client.hpp"

class Client;

class Command {
protected:
	std::vector<std::string>	_args;
public:
	Command();
	Command(const Command &command);

	~Command();

	Command	&operator=(const Command &command);

	virtual bool	execute(Client &executor, std::vector<std::string> &args) const = 0;
};