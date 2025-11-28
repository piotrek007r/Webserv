structs that are in netinet/in.h
struct in_addr {
    uint32_t s_addr;  // 32-bit IPv4 address in network byte order
};

struct sockaddr_in {
    sa_family_t    sin_family;   // Address family, e.g., AF_INET
    in_port_t      sin_port;     // Port number (network byte order)
    struct in_addr sin_addr;     // IPv4 address
    unsigned char  sin_zero[8];  // Padding, usually zeroed
};

------------

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

--------------