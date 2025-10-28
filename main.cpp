#include <iostream>
#include <sys/socket.h>
#include <stdint.h>
#include <arpa/inet.h>    // inet_pton, inet_ntop
#include <netinet/in.h>   // struct sockaddr_in, struct in_addr

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

   in_addr in_addr;
   
   int status = inet_pton(AF_INET, "192.168.8.104", &in_addr);
   switch (status)
    {
        case 0:
        std::cerr << "Error: invalid IP string" << std::endl;
        return 0;
        case -1:
        std::cerr << "Error: invalid address family" << std::endl;
        return 0;
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
       
       sockaddr sockaddr;
       sockaddr_in sockaddr_in {};
       sockaddr_in.sin_family = AF_INET;
       sockaddr_in.sin_port = htons(8080);
       sockaddr_in.sin_addr = in_addr;
       
       // binding socker FD to IP+port stored in sockaddr_in, similar FD <-> pipe end;
       int statusBind = bind(socketFd, (struct sockaddr*)&sockaddr_in, sizeof(sockaddr_in));
       if(statusBind == -1)
       {
           std::cerr << "Error: Failed to bind socket" << std::endl;
           return 0;
        }
        
        int statusLis = listen(socketFd, 5);
        if(statusLis == -1)
        {
            std::cerr << "Error: ..." << std::endl;
            return 0;
        }
        // getsockname() // testing
    while(true)
    {
    }
}

/*
# From the same machine or another machine on the network:
telnet <IP> <PORT>
nc <IP> <PORT>
*/