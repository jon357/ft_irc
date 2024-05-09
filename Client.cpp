#include "Client.hpp"


Client::Client(int fd, std::string name) : _fd(fd), _name(name)
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

void    Client::setfd()
{

}

void    Client::setname()
{

}
