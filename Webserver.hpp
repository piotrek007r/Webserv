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
    addrinfo hints; // base config of server
    addrinfo *serv; 
    sockaddr_storage clientSockaddr; // information about client
    std::vector<int> socketFD; // vector holding all socketsFD
    std::vector<bool> isClientFD; // consider switching it to map
    pollfd *pollFDs; // struct that holds data about socketFD, its events and responses to it
    size_t lSockNum; // number of listening sockets
    int poolTimeout; // time interval for poll() checking for event
    size_t pollFDsNum; // number of structs corresponding total number of sockets
    size_t bakclogNum; // number of pending connection that listen can hold
    // temporary
    char buffer[1024]; // data stream accapted by recv()
    int byteRecived; // number of bytes accepted by recv()

public:
    Webserver();
    void setSocket(size_t i);
    void setConnection(size_t i);
    void recivNClose(size_t i);
    void sendToClient(size_t i);
    void tempLogic();
};

#endif