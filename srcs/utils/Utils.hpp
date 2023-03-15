#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "../channel/Channel.hpp"
#include "../client/Client.hpp"
#include "../server/Server.hpp"

class Server;

bool isNumber(std::string str);
bool isPrintable(std::string str);
bool isValidChannel(std::string str);

std::vector<std::string> split(std::string &str, char delim);

Client  *clientExists(Server *server, std::string name);
Channel *channelExists(Server *server, std::string name);

void	*readFromConsole(void *attr);
std::string getCompleteArgument(std::vector<std::string> args, int n);