#pragma once
// #include "Channel.hpp"
#include <string>
#include <vector>

class Client
{
    public:
        Client(int fd, std::string name);

        int getfd();
        std::string getname();

        void    setfd();
        void    setname();

    private:
        int _fd;
        std::string _name;
        // std::vector<Channel>    _clientChannelList;
};