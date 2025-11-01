#include <iostream>
#include <string>
#include <sys/socket.h>   // for sockets
#include <stdint.h>
#include <arpa/inet.h>    // inet_pton, inet_ntop
#include <netinet/in.h>   // struct sockaddr_in, struct in_addr
#include <cerrno>         // defines errno
#include <cstring>        // defines strerror()
#include <unistd.h>       // for close read write
// #include <sstream>

int main()
{

    /* structs that are in netinet/in.h

        struct in_addr {
            uint32_t s_addr;  // 32-bit IPv4 address in network byte order
        };

        struct sockaddr_in {
            sa_family_t    sin_family;   // Address family, e.g., AF_INET
            in_port_t      sin_port;     // Port number (network byte order)
            struct in_addr sin_addr;     // IPv4 address
            unsigned char  sin_zero[8];  // Padding, usually zeroed
        };

    */

   in_addr server_ip;
   
   int status = inet_pton(AF_INET, "0.0.0.0", &server_ip);
   switch (status)
    {
        case 0:
        std::cerr << "Error: invalid IP string" << std::endl;
        return 1;
        case -1:
        std::cerr << "Error: invalid address family" << std::endl;
        return 1;
        default:
        break;
    }
    /* 
    family: 
    AF_INET -> IPv4  
    AF_INET6 -> IPv6
    AF_UNSPEC -> neither IPv4/IPv6 for getaddinfo (get available addres)
    socktype:
    SOCK_STREAM -> TCP
    SOCK_DGRAM -> UDP
    SOCK_RAW -> for manual control, for networking tools
    protocol:
    Usually 0 for standard socktype but for SOCK_RAW there is IPPROTO_ICMP 
    */
       
    // opening a FD for socket of given 2 params 
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    
    std::cout << "Socket FD: " << socketFd << std::endl; 
    
    // sockaddr sockaddr;
    sockaddr_in servSockaddr_in; // <- server struct 
    memset(&servSockaddr_in, 0, sizeof(sockaddr_in));
    sockaddr_storage clientSockaddr;
    memset(&clientSockaddr, 0, sizeof(sockaddr_in));
    servSockaddr_in.sin_family = AF_INET;
    servSockaddr_in.sin_port = htons(8080);
    servSockaddr_in.sin_addr = server_ip;
    
    int option = 1;
    // reles soket from kernel hold, allow to multile use after close
    setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    /*
    binding socker FD to IP+port stored in sockaddr_in, similar FD <-> pipe end;
    casting to defoult struct sockaddr that can work with any kind of those structs
    */
    int statusBind = bind(socketFd, (struct sockaddr*)&servSockaddr_in, sizeof(servSockaddr_in));
    if(statusBind == -1)
    {
        std::cerr << "bind() failed: " << strerror(errno) << std::endl;
        return 1;
    }
    // last param for listen in number of connection allowed on the incoming queue
    int statusLis = listen(socketFd, 5);
    if(statusLis == -1)
    {
        std::cerr << "listen() failed: " << strerror(errno) << std::endl;
        return 1;
    }
    std::cout << "accept connection: " << std::endl;
    /*
    addrlen is set in front and it can change due to different sockaddr struct,
    Kernel need to know how big struct gonna be;
    */
    socklen_t addrLen = sizeof(sockaddr_storage);
    int clientFD = accept(socketFd, (struct sockaddr*)&clientSockaddr, &addrLen);
    if(clientFD == -1)
    {
        std::cerr << "accept() failed: " << strerror(errno) << std::endl;
        return 1;
    }
    std::string str = "Hello this is server talking to U!\n";
    int byteSend = send(clientFD, str.c_str(), str.size(), 0); // check if string functions are ok 
    if(byteSend == -1)
    {
        std::cerr << "send() failed: " << strerror(errno) << std::endl;
        return 1;
    }
    char buffer[1024];
    // std::istringstream iss;
    int byteRecived;
    while(true)
    {
        byteRecived = recv(clientFD, &buffer, 1024, 0);
        buffer[byteRecived] = '\0'; // -2 coz telnet send additional /r+/n while confirm 
        if(byteRecived == -1)
        {
            std::cerr << "send() failed: " << strerror(errno) << std::endl;
            return 1;
        }
        else if (byteRecived == 0)
            break;
        std::cout << byteRecived << std::endl;
        std::cout << buffer << std::endl;
    }
    close(socketFd);
    close(clientFD);
        // getsockname() // testing
    // while(true)
    // {
    // }
    return 0;
}

/*
# From the same machine or another machine on the network:
telnet <IP> <PORT>
nc <IP> <PORT>
*/