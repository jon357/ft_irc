#pragma once
#include "Client.hpp"

class Channel
{
    private:
        std::string _name;
        std::string _topic;
        std::vector<Client>    _channelClientList;

    public:
        Channel(int fd);
};