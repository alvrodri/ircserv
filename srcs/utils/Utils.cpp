
#include "./Utils.hpp"

bool isNumber(std::string str) {
    for (unsigned long i = 0; i < str.length(); i++) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool isPrintable(std::string str) {
    for (unsigned long i = 0; i < str.length(); i++) {
        if (!std::isprint(str[i])) {
            return false;
        }
    }
    return true;
}

bool isValidChannel(std::string str) {
    if (str[0] != '#' && str[0] != '&') {
        return false;
    }

    if (str.size() > 200) {
        return false;
    }

    if (str.find(' ') != std::string::npos) {
        return false;
    }

    if (str.find(7) != std::string::npos) {
        return false;
    }

    if (str.find(',') != std::string::npos) {
        return false;
    }

    return true;
}

std::vector<std::string> split(std::string &str, char delim) {
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start));
    }
    return strings;
}

Client *clientExists(Server *server, std::string name) {
	for (unsigned long i = 0; i < server->getClients().size(); i++) {
		if (server->getClients()[i].getNick() == name) {
			return &server->getClients()[i];
		}
	}
	return NULL;
}

Channel	*channelExists(Server *server, std::string name) {
	for (unsigned long i = 0; i < server->getClients().size(); i++) {
		if (server->getChannels().find(name) != server->getChannels().end()) {
			return server->getChannels().find(name)->second;
		}
	}
	return NULL;
}

void	*readFromConsole(void *attr) {
	Server *server = (Server *)attr;

	std::string input;
    while (getline(std::cin, input) && input != "quit") {
		if (input == "channels") {
            std::cout << "-- Channels (" << server->getChannels().size() <<  ") -- " << std::endl;
            for (Server::t_channels::iterator it = server->getChannels().begin(); it != server->getChannels().end(); it++) {
                std::cout << it->first << ": " << it->second->getClients().size() << std::endl;
            }
		}
    }

	pthread_exit(NULL);
}

std::string getCompleteArgument(std::vector<std::string> args, int n) {
    std::string ret = "";

    for (unsigned long i = n; i < args.size(); i++) {
		ret += args[i];
		if (i != args.size() - 1)
			ret += " ";
	}

	if (ret[0] == ':') {
		ret.erase(ret.begin());
	}
    return ret;
};
