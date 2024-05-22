#include "Client.hpp"


Client::Client()
{
}

Client::Client(int fd, std::string name, std::string user) : _fd(fd), _name(name), _user(user)
{
	_logged = false;
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

bool Client::getlogged()
{
	return (_logged);
}


void    Client::setfd()
{

}

void    Client::setname(std::string name)
{
	_name = name;
}
void    Client::setuser(std::string user)
{
    _user = user;

}

void    Client::setlogged()
{
	_logged = true;
}
