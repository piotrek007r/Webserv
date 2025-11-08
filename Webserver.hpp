#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <arpa/inet.h> // inet_pton, inet_ntop DELETE if dont need
#include <cerrno>      // defines errno
#include <cstring>     // defines strerror()
#include <fcntl.h>     // to set NONBLOCK
#include <iostream>
#include <netinet/in.h> // struct sockaddr_in, struct in_addr
#include <netdb.h>      // for getaddrinfo and addrinfo
#include <poll.h>       // for poolfd
#include <string>
#include <stdint.h>
#include <stdlib.h>     // for exit
#include <sys/socket.h> // for sockets
#include <unistd.h>     // for close read write
#include <vector>

class Webserver
{
private:
    addrinfo hints;
    addrinfo *serv;
    sockaddr_storage clientSockaddr;
    std::vector<int> socketFD;
    std::vector<bool> isClientFD; // consider switching it to map
    pollfd *pollFDs;
    size_t lSockNum; // number of listening sockets
    int poolTimeout;
    size_t pollFDsNum; // number of structs corresponding total number of sockets
    // temporary
    char buffer[1024]; 
    int byteRecived;

public:
    Webserver();
    void setSocket(size_t i);
    void setConnection(size_t i);
    void recivNClose(size_t i);
    void sendToClient(size_t i);
    void tempLogic();
};

#endif