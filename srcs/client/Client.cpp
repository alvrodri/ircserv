#include "Client.hpp"
#include <algorithm>

Client::Client() {
}

Client::Client(Server *server, struct pollfd poll): _server(server), _poll(poll), _modes(""), _registered(false), _currentChannels() {
}

Client::Client(const Client &client) {
	this->operator=(client);
}

Client::~Client() {
}

Client	&Client::operator=(const Client &client) {
	this->_server = client._server;
	this->_poll = client._poll;
	this->_modes = client._modes;
	return *this;
}

void	Client::executeMessage() {
	std::vector<std::string> args = split(this->_message, ' ');
	std::string	message = this->_message.substr(0, this->_message.find_first_of(' '));

	Command *command = NULL;
	command = this->_server->getCommands()[message];

	if (command) {
		//std::cout << "Executing |" << message << "|" << std::endl;
		command->execute(*this, args);
	}
}

std::string	Client::getNick() const {
	return this->_nick;
}

void	Client::setNick(std::string nick) {
	this->_nick = nick;
}

std::string	Client::getUsername() const {
	return this->_username;
}

void	Client::setUsername(std::string username) {
	this->_username = username;
}

std::string	Client::getHostname() const {
	return this->_hostname;
}

void	Client::setHostname(std::string hostname) {
	this->_hostname = hostname;
}

std::string	Client::getServername() const {
	return this->_servername;
}

void	Client::setServername(std::string servername) {
	this->_servername = servername;
}

std::string	Client::getRealname() const {
	return this->_realname;
}

void	Client::setRealname(std::string realName) {
	this->_realname = realName;
}

void	Client::setMode(char mode, bool status) {
	if (status) {
		if (this->hasMode(mode)) {
			return;
		}

		this->_modes.push_back(mode);
		return;
	}

	if (!this->hasMode(mode)) {
		return;
	}

	this->_modes.erase(std::remove(this->_modes.begin(), this->_modes.end(), mode), this->_modes.end());
}

bool	Client::hasMode(char mode) {
	return this->_modes.find(mode) != this->_modes.npos;
}

const std::string &Client::getModes() const {
	return this->_modes;
}

std::string	&Client::getMessage() {
	return this->_message;
}

void	Client::setMessage(std::string message) {
	this->_message = message;
}

struct pollfd	Client::getPoll() const {
	return this->_poll;
}

Server			*Client::getServer() const {
	return this->_server;
}

bool			Client::getRegistered() const {
	return this->_registered;
}

void			Client::setRegistered() {
	Server		*server = this->_server;

	this->_registered = true;
	server->reply(*this, "RPL_WELCOME", ":Welcome to the Internet Relay Network, " + this->_nick);
	server->reply(*this, "RPL_YOURHOST", ":Your host is ftirc, running version 1.0");
	server->reply(*this, "RPL_CREATED", ":This server was created today");
	server->reply(*this, "RPL_MYINFO", ":ftirc 0.1 aiwroOs aiwroOs");
	server->reply(*this, "RPL_MOTDSTART", ":- ftirc Message of the day - ");
	server->reply(*this, "RPL_MOTD", ":- Buenos días!");
	server->reply(*this, "RPL_ENDOFMOTD", ":End of /MOTD command");
}

bool			Client::getAuthenticated() const {
	return this->_authenticated;
}

void            Client::setAuthenticated() {
	this->_authenticated = true;
}

std::vector<Channel *>	&Client::getCurrentChannels() {
	return this->_currentChannels;
}

int			Client::joinChannel(std::string name) {
	Server	*server = this->getServer();

	if (!isValidChannel(name)) {
		return 476;
	}

	Channel	*channel = NULL;
	if (server->getChannels().find(name) == server->getChannels().end()) {
		channel = new Channel(name);
		server->getChannels().insert(std::make_pair(name, channel));
		channel->setClientMode(this, 'o', true);
	} else {
		channel = server->getChannels().find(name)->second;
	}

	if (std::find(this->getCurrentChannels().begin(), this->getCurrentChannels().end(), channel) != this->getCurrentChannels().end()) {
		return 1;
	}

	channel->getClients().push_back(this);
	this->_currentChannels.push_back(channel);
	return -1;
}

void	Client::leaveChannel(std::string name) {
	this->leaveChannel(name, false);
}

void	Client::leaveChannel(std::string name, bool kicked) {
	for (unsigned long i = 0; i < this->_currentChannels.size(); i++) {
		if (this->_currentChannels[i]->getName() == name) {
			Channel *channel = this->_currentChannels[i];

			if (!kicked)
				this->_server->simpleReply(channel->getClients(), ":" + this->getNick() + " PART " + channel->getName());

			this->_currentChannels.erase(std::find(this->_currentChannels.begin(), this->_currentChannels.end(), channel));
			channel->getClients().erase(std::find(channel->getClients().begin(), channel->getClients().end(), this));
			if (channel->getClients().size() <= 0) {
				this->_server->getChannels().erase(channel->getName());
				delete channel;
			} else {
				channel->setClientMode(channel->getClients()[0], 'o', true);
				this->_server->simpleReply(channel->getClients(), ":ftirc MODE " + channel->getName() + " +o " + channel->getClients()[0]->getNick());
			}

			break;
		}
	}
}

void	Client::leaveChannels() {
	for (unsigned long i = 0; i < this->_currentChannels.size(); i++) {
		Channel *channel = this->_currentChannels[i];

		this->_server->simpleReply(channel->getClients(), ":" + this->getNick() + " PART " + channel->getName());

		this->_currentChannels.erase(std::find(this->_currentChannels.begin(), this->_currentChannels.end(), channel));
		channel->getClients().erase(std::find(channel->getClients().begin(), channel->getClients().end(), this));
		if (channel->getClients().size() <= 0) {
			this->_server->getChannels().erase(channel->getName());
			delete channel;
		} else {
			channel->setClientMode(channel->getClients()[0], 'o', true);
			this->_server->simpleReply(channel->getClients(), ":ftirc MODE " + channel->getName() + " +o " + channel->getClients()[0]->getNick());
		}
	}
}
