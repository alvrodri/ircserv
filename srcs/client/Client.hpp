#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#include "../server/Server.hpp"
#include "../utils/Utils.hpp"
#include "../channel/Channel.hpp"

/*
	A client is anything connecting to a server that is not another
	server.  Each client is distinguished from other clients by a unique
	nickname having a maximum length of nine (9) characters.  See the
	protocol grammar rules for what may and may not be used in a
	nickname.  In addition to the nickname, all servers must have the
	following information about all clients: the real name of the host
	that the client is running on, the username of the client on that
	host, and the server to which the client is connected.
*/

class Server;
class Channel;

class Client {
private:
	Server	*_server;

	struct pollfd	_poll;

	std::string	_nick;
	std::string	_username;
	std::string	_hostname;
	std::string	_servername;
	std::string	_realname;
	std::string	_message;

	public:
	std::string	_modes;

	private:
	bool					_registered;
	bool					_authenticated;

	std::vector<Channel *>	_currentChannels;
public:
	Client();
	Client(Server *server, struct pollfd poll);
	Client(const Client &client);

	~Client();

	void		executeMessage();

	std::string	&getMessage();
	void		setMessage(std::string message);

	std::string	getNick() const;
	void		setNick(std::string nick);

	std::string	getUsername() const;
	void		setUsername(std::string username);

	std::string	getRealname() const;
	void		setRealname(std::string realname);

	std::string	getHostname() const;
	void		setHostname(std::string hostname);

	std::string	getServername() const;
	void		setServername(std::string servername);

	void				setMode(char mode, bool status);
	bool				hasMode(char mode);
	const std::string	&getModes() const;

	int		joinChannel(std::string name);
	void	leaveChannel(std::string name);
	void	leaveChannel(std::string name, bool kicked);
	void	leaveChannels();

	struct pollfd	getPoll() const;

	Server			*getServer() const;

	bool			getRegistered() const;
	void			setRegistered();

	bool			getAuthenticated() const;
	void            setAuthenticated();

	std::vector<Channel *>	&getCurrentChannels();
private:
	Client	&operator=(const Client &client);
};