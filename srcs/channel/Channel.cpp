#include "Channel.hpp"

Channel::Channel(std::string name) {
    this->_name = name;
}

Channel::~Channel() {

}

std::vector<Client *>   &Channel::getClients() {
    return this->_clients;
}

std::string &Channel::getName() {
    return this->_name;
}

std::string &Channel::getTopic() {
    return this->_topic;
}

void    Channel::setTopic(std::string topic) {
    this->_topic = topic;
}

std::map<Client *, std::string>	&Channel::getClientModes() {
    return this->_clientModes;
}

bool	Channel::hasClientMode(Client *client, char mode) {
    if (this->getClientModes().find(client) == this->getClientModes().end()) {
        return false;
    }

    std::string &modes = this->getClientModes().find(client)->second;

    return modes.find(mode) != modes.npos;
}

void    Channel::setClientMode(Client *client, char mode, bool status) {
    if (!client) {
        return;
    }

    if (this->getClientModes().find(client) == this->getClientModes().end()) {
        this->getClientModes().insert(std::make_pair(client, ""));
    }

    std::string &modes = this->getClientModes().find(client)->second;

    if (status) {
        if (this->hasClientMode(client, mode)) return;

        modes.push_back(mode);
        return;
    }

    if (!this->hasClientMode(client, mode)) return;

	modes.erase(std::remove(modes.begin(), modes.end(), mode), modes.end());
}

bool	Channel::hasMode(char mode) {
    return this->_modes.find(mode) != this->_modes.npos;
}

void    Channel::setMode(char mode, bool status) {
    if (status) {
        if (this->hasMode(mode)) return;

        this->_modes.push_back(mode);
        return;
    }

    if (!this->hasMode(mode)) return;

	this->_modes.erase(std::remove(this->_modes.begin(), this->_modes.end(), mode), this->_modes.end());
}

std::string &Channel::getModes() {
    return this->_modes;
}