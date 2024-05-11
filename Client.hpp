#pragma once
// #include "Channel.hpp"
#include <string>
#include <vector>

class Client
{
    public:
        Client(int fd, std::string name, std::string user);

        int getfd();
        std::string getname();
        std::string getuser();

        void    setfd();
        void    setname();

    private:
        int _fd;
        std::string _name;
        std::string _user;
        // std::vector<Channel>    _clientChannelList;
};