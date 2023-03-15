#pragma once

#include "../Command.hpp"

class TopicCommand : public Command {
public:
	TopicCommand();
	~TopicCommand();

	TopicCommand(const TopicCommand &command);

	TopicCommand	&operator=(const TopicCommand &command);

	bool	execute(Client &executor, std::vector<std::string> &args) const;
};