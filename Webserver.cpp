#include "Webserver.hpp"

Webserver::Webserver() : serv(NULL), pollFDs(NULL), pollFDsNum(0), poolTimeout(1000)
{
   memset(&clientSockaddr, 0, sizeof(sockaddr_in));
   memset(&hints, 0, sizeof(sockaddr_in));
   hints.ai_family = AF_UNSPEC;     // for both IP4 & IP6
   hints.ai_socktype = SOCK_STREAM; // TCP
   hints.ai_flags = AI_PASSIVE;     // for server means NULL will accept connection from any adress
}

void Webserver::tempLogic()
{
   getaddrinfo(NULL, "8080", &hints, &serv);

   int option = 1;
   size_t i = 0;
   while (true)
   {
      socketFD.push_back(socket(serv->ai_family, SOCK_STREAM, 0)); // error check missing
      // release soket from kernel hold, allow to multile use after close
      if (setsockopt(socketFD[i], SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
      {
         std::cerr << "setsockopt() failed: " << strerror(errno) << std::endl;
         exit(1);
      }
      if (serv->ai_family == AF_INET6)
      {
         int yes = 1;
         // Make this IPv6 socket handle only IPv6 addresses, preventing conflicts with IPv4 sockets
         if (setsockopt(socketFD[i], IPPROTO_IPV6, IPV6_V6ONLY, &yes, sizeof(yes)) == -1)
         {
            std::cerr << "setsockopt() failed: " << strerror(errno) << std::endl;
            exit(1);
         }
      }
      fcntl(socketFD[i], F_SETFL, O_NONBLOCK);
      /*binding socket FD to IP+port stored in erv->ai_addr, similar FD <-> pipe end;
      casting to defoult struct sockaddr that can work with any kind of those structs*/
      if (bind(socketFD[i], serv->ai_addr, serv->ai_addrlen) == -1)
      {
         std::cerr << "bind() failed: " << strerror(errno) << std::endl;
         exit(1);
      }
      // last param for listen in number of connection allowed on the incoming queue
      if (listen(socketFD[i], 5) == -1)
      {
         std::cerr << "listen() failed: " << strerror(errno) << std::endl;
         exit(1);
      }
      isListenFD.push_back(true);
      pollFDs = (pollfd *)realloc(pollFDs, (i + 1) * sizeof(pollfd));
      pollFDs[i].fd = socketFD[i];
      pollFDs[i].events = POLLIN;
      pollFDsNum++;
      i++;
      if (serv->ai_next == NULL)
         break;
      serv = serv->ai_next;
   }

   /*
   Plan is to first set poll to recive by setiing every event to POLLIN.
   Later when logic works implement for sending with such optimization:
   - ready to send -> find corresponding socket with pollFD struct
      -> change its event to POLLOUT -> send -> remove POLLOUT from event
   */


   while (true)
   {
      int pollstatus = poll(pollFDs, pollFDsNum, poolTimeout);
      if (pollstatus > 0)
      {
         for (int i = 0; i < pollFDsNum; i++)
         {
            if ((pollFDs[i].revents & POLLIN) && isListenFD[i])
            {
               isListenFD[i] = false; 
               pollFDs[i].events = POLLOUT;
               // set new flag to recv and send
               // accept connection
               std::cout << "socket: " << pollFDs[i].fd << " ready to connect" << std::endl;
            }
            else if (pollFDs[i].revents & POLLIN)
            {
               std::cout << "client: " << pollFDs[i].fd << " ready to recv" << std::endl;
            }
            std::cout << "--" << pollFDs[i].revents << std::endl;
            if (pollFDs[i].revents & POLLOUT)
            {
               std::cout << "client: " << pollFDs[i].fd << " ready to send" << std::endl;
            }
            if (pollFDs[i].revents & POLLHUP)
            {
               std::cout << "client on FD: " << pollFDs[i].fd << std::endl;
               // close this client client
            }
            if (pollFDs[i].revents & POLLERR)
            {
               std::cout << "poll() failed: revent return POLLERR" << std::endl;
               exit(1); // propably to remove
            }
         }
      }
      usleep(1000000);
      // std::cout << pollstatus << std::endl;
      // std::cout << i << std::endl;
   }

   // --------> blocked connection test <----------

   // socklen_t addrLen = sizeof(sockaddr_storage);
   // int clientFD = accept(socketFD[0], (struct sockaddr *)&clientSockaddr, &addrLen);
   // if (clientFD == -1)
   // {
   //    std::cerr << "accept() failed: " << strerror(errno) << std::endl;
   //    exit(1);
   // }
   // std::cout << "accept connection: " << std::endl;
   // std::string str = "Hello this is server talking to U!\n";
   // int byteSend = send(clientFD, str.c_str(), str.size(), 0); // check if string functions are ok
   // if (byteSend == -1)
   // {
   //    std::cerr << "send() failed: " << strerror(errno) << std::endl;
   //    exit(1);
   // }
   // char buffer[1024];
   // int byteRecived;
   // while (true)
   // {
   //    byteRecived = recv(clientFD, &buffer, 1024, 0);
   //    buffer[byteRecived] = '\0'; // -2 coz telnet send additional /r+/n while confirm
   //    if (byteRecived == -1)
   //    {
   //       std::cerr << "send() failed: " << strerror(errno) << std::endl;
   //       exit(1);
   //    }
   //    else if (byteRecived == 0)
   //       break;
   //    std::cout << byteRecived << std::endl;
   //    std::cout << buffer << std::endl;
   // }
   close(socketFD[0]);
   close(socketFD[1]);
   // close(clientFD);
   freeaddrinfo(serv);
}