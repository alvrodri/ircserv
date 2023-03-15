#include "Server.hpp"

Server::Server() {
	throw std::runtime_error("Not implemented");
}

Server::Server(int port, const std::string &password) : _port(port), _password(password), _commandManager() {
	int	i;

	this->_commandManager.registerCommands();
	this->registerReplies();

	if ((this->_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		throw std::runtime_error("ft_irc server: " + std::string(strerror(errno)));
	}

	sockaddr_in	sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(this->_port);

	if (fcntl(this->_sockfd, F_SETFL, O_NONBLOCK) == -1) {
		throw std::runtime_error("ft_irc server: " + std::string(strerror(errno)));
	}

	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int)) == -1) {
		throw std::runtime_error("ft_irc server: " + std::string(strerror(errno)));
	}

	if (bind(this->_sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1) {
		throw std::runtime_error("ft_irc server: " + std::string(strerror(errno)));
	}

	if (listen(this->_sockfd, 32) == -1) {
		throw std::runtime_error("ft_irc server: " + std::string(strerror(errno)));
	}

	this->_poll.push_back((struct pollfd) { this->_sockfd, POLLIN, 0 } );

	std::cout << "ft_irc server is ready to accept connections on port " << this->_port << " with password " << this->_password << "." << std::endl;
	
	pthread_t readFromConsoleThread;
	pthread_create(&readFromConsoleThread, NULL, readFromConsole, this);

	this->startListening();

	pthread_join(readFromConsoleThread, NULL);
}

Server::~Server() {
	close(this->_sockfd);
}

std::vector<struct pollfd>	&Server::getPoll() {
	return this->_poll;
}

std::map<int, Client>	&Server::getClients() {
	return this->_clients;
}

std::map<std::string, std::string>	&Server::getReplies() {
	return this->_replies;
}

CommandManager	&Server::getCommandManager() {
	return this->_commandManager;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void	Server::startListening() {
	struct sockaddr_storage remoteaddr;

	while (true) {
		int	poll_count = poll(this->_poll.begin().base(), this->_poll.size(), -1);

		if (poll_count < 0) {
			std::cout << "Error polling fds." << std::endl;
			exit(1);
		}

		for (int i = 0; i < this->_poll.size(); i++) {
			if (this->_poll[i].revents & POLLIN) {
				if (this->_poll[i].fd == this->_sockfd) {
					socklen_t	addrlen = sizeof(remoteaddr);

					int	newfd = accept(this->_sockfd, (struct sockaddr *) &remoteaddr, &addrlen);

					if (newfd < 0) {
						std::cout << "Error accepting a new connection." << std::endl;
					} else {
						fcntl(newfd, F_SETFL, O_NONBLOCK);
						this->_poll.push_back((struct pollfd) { newfd, POLLIN, 0 } );
						this->_clients.insert(std::make_pair(newfd, Client(this, this->_poll.back())));
						std::cout << "Accepted new connection! (" << this->_clients.size() << ")" << std::endl;
					}
				} else {
					char buffer[8];
					int bytes = recv(this->_poll[i].fd, buffer, sizeof(buffer), 0);
					std::string message;

					if (bytes <= 0) {
						close(this->_poll[i].fd);
						this->_clients[this->_poll[i].fd].leaveChannels();
						this->_clients.erase(this->_poll[i].fd);
						this->_poll.erase(this->_poll.begin() + i);
						break ;
					}

					while (bytes > 0) {
						message.append(buffer, bytes);
						bytes = recv(this->_poll[i].fd, buffer, sizeof(buffer), 0);
					}
					this->_clients[this->_poll[i].fd].setMessage(this->_clients[this->_poll[i].fd].getMessage().append(message));

					int rpos = 0;
					std::string &command = this->_clients[this->_poll[i].fd].getMessage();
					while ((rpos = command.find('\r')) != std::string::npos) {
						std::string	commandToExecute = command.substr(0, rpos);
						std::string restOfCommand = command.substr(rpos + (command[rpos + 1] == '\n' ? 2 : 1));

						this->_clients[this->_poll[i].fd].setMessage(commandToExecute);
						this->_clients[this->_poll[i].fd].executeMessage();
						this->_clients[this->_poll[i].fd].setMessage(restOfCommand);
					}
				}
			}
		}
	}
}

void	Server::reply(const Client &client, std::string reply, std::string message) {
	std::string	messageToSend = ":ftirc " + (reply.empty() ? "" : this->_replies[reply] + " ");

	if (client.getNick().empty())
		messageToSend += "*";
	else {
		if (!reply.empty()) {
			messageToSend += client.getNick();
		}
	}
	messageToSend += (" " + message + "\r\n");
	std::cout << "|" << messageToSend << "|" << std::endl;

	send(client.getPoll().fd, messageToSend.c_str(), messageToSend.size(), 0);
}

void	Server::reply(std::vector<Client *> clients, std::string reply, std::string message) {
	for (int i = 0; i < clients.size(); i++) {
		this->reply(*clients[i], reply, message);
	}
}

void	Server::simpleReply(const Client &client, std::string message) {
	std::string messageToSend = message;
	messageToSend += "\r\n";

	send(client.getPoll().fd, messageToSend.c_str(), messageToSend.size(), 0);
}

void	Server::simpleReply(std::vector<Client *> clients, std::string message) {
	for (int i = 0; i < clients.size(); i++) {
		this->simpleReply(*clients[i], message);
	}
}

void	Server::sendMessage(const Client &client, std::string message) {
	message += "\r\n";
	send(client.getPoll().fd, message.c_str(), message.size(), 0);
}

void	Server::sendMessage(Channel &channel, std::string message) {
	for (int i = 0; i < channel.getClients().size(); i++) {
		this->sendMessage(*channel.getClients()[i], message);
	}
}

void	Server::sendMessage(Client &except, Channel &channel, std::string message) {
	for (int i = 0; i < channel.getClients().size(); i++) {
		if (except.getNick() != (*channel.getClients()[i]).getNick())
			this->sendMessage(*channel.getClients()[i], message);
	}
}

void	Server::registerReplies() {
	this->_replies.insert(std::make_pair("RPL_WELCOME", "001"));
	this->_replies.insert(std::make_pair("RPL_YOURHOST", "002"));
	this->_replies.insert(std::make_pair("RPL_CREATED", "003"));
	this->_replies.insert(std::make_pair("RPL_MYINFO", "004"));

	this->_replies.insert(std::make_pair("ERR_NEEDMOREPARAMS", "461"));
	this->_replies.insert(std::make_pair("ERR_CHANOPRIVSNEEDED", "482"));
	this->_replies.insert(std::make_pair("ERR_NOSUCHNICK", "401"));
	this->_replies.insert(std::make_pair("ERR_NORECIPIENT", "411"));
	this->_replies.insert(std::make_pair("ERR_NOTEXTTOSEND", "412"));
	this->_replies.insert(std::make_pair("ERR_NOSUCHCHANNEL", "403"));
	this->_replies.insert(std::make_pair("ERR_UNKNOWNMODE", "472"));
	this->_replies.insert(std::make_pair("ERR_TOOMANYTARGETS", "407"));
	this->_replies.insert(std::make_pair("ERR_ALREADYREGISTRED", "462"));
	this->_replies.insert(std::make_pair("ERR_NONICKNAMEGIVEN", "431"));
	this->_replies.insert(std::make_pair("ERR_ERRONEUSNICKNAME", "432"));
	this->_replies.insert(std::make_pair("ERR_NICKNAMEINUSE", "433"));
	this->_replies.insert(std::make_pair("ERR_UMODEUNKNOWNFLAG", "501"));
	this->_replies.insert(std::make_pair("ERR_USERSDONTMATCH", "502"));
	this->_replies.insert(std::make_pair("ERR_BADCHANMASK", "476"));
	this->_replies.insert(std::make_pair("ERR_USERNOTINCHANNEL", "441"));
	this->_replies.insert(std::make_pair("ERR_CHANOPRIVSNEEDED", "482"));

	this->_replies.insert(std::make_pair("RPL_UMODEIS", "221"));
	this->_replies.insert(std::make_pair("RPL_NOTOPIC", "331"));
	this->_replies.insert(std::make_pair("RPL_TOPIC", "332"));
	this->_replies.insert(std::make_pair("RPL_NAMREPLY", "353"));
	this->_replies.insert(std::make_pair("RPL_ENDOFNAMES", "366"));
	this->_replies.insert(std::make_pair("RPL_MOTDSTART", "375"));
	this->_replies.insert(std::make_pair("RPL_MOTD", "372"));
	this->_replies.insert(std::make_pair("RPL_ENDOFMOTD", "376"));
	this->_replies.insert(std::make_pair("RPL_LISTSTART", "321"));
	this->_replies.insert(std::make_pair("RPL_LIST", "322"));
	this->_replies.insert(std::make_pair("RPL_LISTEND", "323"));
	this->_replies.insert(std::make_pair("RPL_CHANNELMODEIS", "324"));
}

const std::string&	Server::getPassword() const {
	return this->_password;
}

std::map<std::string, Channel *>	&Server::getChannels() {
	return this->_channels;
}

void	Server::sendNames(Client &client, Channel &channel) {
	std::string	clients = channel.getClients()[0]->getNick() + (channel.getClients().size() > 1 ? " " : "");
	
	if (channel.getClients().size() > 1) {
		for (int i = 1; i < channel.getClients().size(); i++) {
			if (!channel.getClients()[i]->hasMode('i'))
				clients += channel.getClients()[i]->getNick() + (i == channel.getClients().size() - 1 ? "" : " ");
		}
	}
	this->reply(client, "RPL_NAMREPLY", "= " + channel.getName() + " :" + clients);
	this->reply(client, "RPL_ENDOFNAMES", channel.getName() + " :End of /NAMES list");
}