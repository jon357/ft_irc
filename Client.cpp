#include "Client.hpp"


Client::Client(int fd, std::string name, std::string user) : _fd(fd), _name(name), _user(user)
{

}

int Client::getfd()
{
    return(_fd);
}

std::string    Client::getname()
{
    return (_name);
}


std::string    Client::getuser()
{
    return (_user);
}

void    Client::setfd()
{

}

void    Client::setname()
{

}
