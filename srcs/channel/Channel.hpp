#pragma once

#include <iostream>
#include <vector>
#include "../client/Client.hpp"

/*
	A channel is a named group of one or more clients which will all
	receive messages addressed to that channel.  The channel is created
	implicitly when the first client joins it, and the channel ceases to
	exist when the last client leaves it.  While channel exists, any
	client can reference the channel using the name of the channel.

	Channels names are strings (beginning with a '&' or '#' character) of
	length up to 200 characters.  Apart from the the requirement that the
	first character being either '&' or '#'; the only restriction on a
	channel name is that it may not contain any spaces (' '), a control G
	(^G or ASCII 7), or a comma (',' which is used as a list item
	separator by the protocol).
*/

class Client;

class Channel {
private:
	std::string						_name;
	std::string						_topic;
	
	std::vector<Client *>			_clients;
	std::map<Client *, std::string>	_clientModes;

	std::string				_modes;

public:
	Channel(std::string name);

	~Channel();

	std::vector<Client *>	&getClients();

	std::string				&getTopic();
	void					setTopic(std::string topic);

	std::string				&getName();

	std::map<Client *, std::string>	&getClientModes();
	std::string				&getModes();

	bool					hasClientMode(Client *client, char mode);
	void					setClientMode(Client *client, char mode, bool status);

	bool					hasMode(char mode);
	void					setMode(char mode, bool status);
};