#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <map>
#include <pthread.h>

#include "../client/Client.hpp"
#include "../command/Command.hpp"
#include "../channel/Channel.hpp"

class Client;
class Command;
class Channel;

class Server {
	public:
		typedef std::map<std::string, Channel *>	t_channels;
	private:
		int									_sockfd;
		int									_port;
		std::string							_password;
		std::vector<struct pollfd>			_poll;
		std::map<int, Client>				_clients;
		std::map<std::string, std::string>	_replies;
		std::map<std::string, Command *> 	_commands;
		t_channels							_channels;

		Server(const Server &server);
		Server &operator=(const Server &server);
	public:
		Server();
		Server(int port, const std::string &password);
		~Server();

		std::vector<struct pollfd>			&getPoll();
		std::map<int, Client>				&getClients();
		std::map<std::string, std::string>	&getReplies();
		std::map<std::string, Command *>	&getCommands();
		std::map<std::string, Channel *>	&getChannels();

		void						reply(const Client &client, std::string reply, std::string message);
		void						reply(std::vector<Client *> clients, std::string reply, std::string message);
		void						simpleReply(const Client &client, std::string message);
		void						simpleReply(std::vector<Client *> clients, std::string message);
		void						sendMessage(const Client &client, std::string message);
		void						sendMessage(Channel &channel, std::string message);
		void						sendMessage(Client &except, Channel &channel, std::string message);
		void						sendNames(Client &client, Channel &channel);

		void	startListening();

		const std::string	&getPassword() const;
	private:
		void	registerCommands();
		void	registerReplies();
};