#pragma once

#include "../Command.hpp"

class NoticeCommand : public Command {
public:
	NoticeCommand();
	~NoticeCommand();

	NoticeCommand(const NoticeCommand &command);

	NoticeCommand	&operator=(const NoticeCommand &command);

	bool	execute(Client &executor, std::vector<std::string> &args) const;
};