#include "Command.hpp"

Command::Command() {
}

Command::~Command() {
}

Command::Command(const Command &command) {
	this->operator=(command);
}

Command	&Command::operator=(const Command &command) {
	this->_args = command._args;
	return *this;
}
