// Copyright [2025] <Piotr Ruszkiewicz> <pruszkie@student.42warsaw.pl>

#ifndef COREENGINE_HPP
#define COREENGINE_HPP

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
#include <sstream>

#include "../configReader/config.hpp"

class CoreEngine
{
    private:
        struct client
        {
            sockaddr_storage clientSockaddr;
            ServerConfig serverCfg;
            char buffer[1024];
            int byteRecived;
            int FD;
        }; // it could go to base object.
        client& getClientByFD(int socketFD);  
        addrinfo hints;                       // base config of server
        std::vector<ServerConfig> serversCfg; // list of object parsed cfgfiles
        addrinfo *serv;
        std::vector<int> socketFD;       // vector holding all socketsFD 
        std::vector<bool> isClientFD;    // consider switching it to map
        std::vector<client> clientVec;
        pollfd *pollFDs;   // struct that holds data about socketFD, its events and responses to it
        size_t lSockNum;   // number of listening sockets
        int poolTimeout;   // time interval for poll() checking for event
        size_t pollFDsNum; // number of structs corresponding total number of sockets
        size_t backlogNum; // number of pending connection that listen can hold

    public:
        CoreEngine(const std::vector<ServerConfig> &serverCfg);
        void setSocket(size_t i);
        void setConnection(size_t i);
        void recivNClose(size_t i);
        void sendToClient(size_t i);
        void coreEngine();
        // std::string getBuffer();
};

#endif