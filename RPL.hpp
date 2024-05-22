#pragma once
#include "Client.hpp"

inline std::string RPL_WELCOME(Client client) {
//   "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
    std::ostringstream oss;
    oss << ":Welcome to the Internet Relay Network, " << client.getname() << "!" << client.getuser() << "@localhost"  << "\r\n";
    return oss.str();
}

/*
RPL_WELCOME
The first message sent after client registration, this message introduces the client to the network. The text used in the last param of this message varies wildly.

Servers that implement spoofed hostmasks in any capacity SHOULD NOT include the extended (complete) hostmask in the last parameter of this reply, either for all clients or for those whose hostnames have been spoofed. This is because some clients try to extract the hostname from this final parameter of this message and resolve this hostname, in order to discover their ‘local IP address’.

Clients MUST NOT try to extract the hostname from the final parameter of this message and then attempt to resolve this hostname. This method of operation WILL BREAK and will cause issues when the server returns a spoofed hostname.

RPL_YOURHOST
  "<client> :Your host is <servername>, running version <version>"
Part of the post-registration greeting, this numeric returns the name and software/version of the server the client is currently connected to. The text used in the last param of this message varies wildly.

RPL_CREATED
  "<client> :This server was created <datetime>"
Part of the post-registration greeting, this numeric returns a human-readable date/time that the server was started or created. The text used in the last param of this message varies wildly.

RPL_MYINFO
  "<client> <servername> <version> <available user modes>
  <available channel modes> [<channel modes with a parameter>]"
Part of the post-registration greeting. Clients SHOULD discover available features using RPL_ISUPPORT tokens rather than the mode letters listed in this reply.

RPL_ISUPPORT
  "<client> <1-13 tokens> :are supported by this server"
*/