#pragma once
// #include "Channel.hpp"
#include <string>
#include <vector>

class Client
{
    public:
	
   		Client();
        Client(int fd, std::string name, std::string user);

        int getfd();
        std::string getname();
        std::string getuser();
        bool getlogged();

        void    setfd();
        void    setname(std::string name);
        void    setuser(std::string user);
        void    setlogged();


    private:
        int _fd;
		bool _logged;
        std::string _name;
        std::string _user;
        // std::vector<Channel>    _clientChannelList;
};