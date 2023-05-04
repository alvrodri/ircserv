#include "NoticeCommand.hpp"

NoticeCommand::NoticeCommand(): Command() {
}

NoticeCommand::~NoticeCommand() {
}

NoticeCommand::NoticeCommand(const NoticeCommand &command) {
	this->operator=(command);
}

NoticeCommand	&NoticeCommand::operator=(const NoticeCommand &command) {
	this->_args = command._args;
	return *this;
}

/*4.4.2 Notice

   Command: NOTICE
   Parameters: <nickname> <text>

   The NOTICE message is used similarly to PRIVMSG.  The difference
   between NOTICE and PRIVMSG is that automatic replies must never be
   sent in response to a NOTICE message.  This rule applies to servers
   too - they must not send any error reply back to the client on
   receipt of a notice.  The object of this rule is to avoid loops
   between a client automatically sending something in response to
   something it received.  This is typically used by automatons (clients
   with either an AI or other interactive program controlling their
   actions) which are always seen to be replying lest they end up in a
   loop with another automaton.
*/

bool	NoticeCommand::execute(Client &executor, std::vector<std::string> &args) const {
	Server	*server = executor.getServer();

	if (!executor.getRegistered()) {
		return true;
	}

	if (args.size() <= 1) {
		return true;
	}

	if (args.size() <= 2) {
		return true;
	}

	std::string text = "";
	for (unsigned long i = 2; i < args.size(); i++) {
		text += args[i];

		if (args[2][0] != ':')
			break;
		
		if (i != args.size() - 1)
			text += " ";
	}

	std::vector<std::string> recipents = split(args[1], ',');
	for (unsigned long i = 0; i < recipents.size(); i++) {
		if (std::count(recipents.begin(), recipents.end(), recipents[i]) > 1) {
			return true;
		}

		if (recipents[i].at(0) != '#' && recipents[i].at(0) != '&') {
			Client	*receiver = clientExists(server, recipents[i]);

			if (!receiver || !receiver->hasMode('s')) {
				return true;
			}

			std::string message = ":" + executor.getNick() + " PRIVMSG " + receiver->getNick() + " " + text;
			server->sendMessage(*receiver, message);
		}
	}

	
	return false;
}
