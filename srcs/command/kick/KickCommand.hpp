#pragma once

#include "../Command.hpp"

class KickCommand : public Command {
public:
	KickCommand();
	~KickCommand();

	KickCommand(const KickCommand &command);

	KickCommand	&operator=(const KickCommand &command);

	bool	execute(Client &executor, std::vector<std::string> &args) const;
};