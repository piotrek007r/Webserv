//Copyright [2025] <Piotr Ruszkiewicz> <pruszkie@student.42warsaw.pl>

#include "CoreEngine.hpp"
#include "../http/Http.hpp"

void CoreEngine::setConnection(size_t i)
{
   socklen_t addrLen = sizeof(sockaddr_storage);
   int clientFD = accept(pollFDs[i].fd, (struct sockaddr *)&clientSockaddr, &addrLen);
   if (clientFD == -1)
   {
      std::cerr << "accept() failed: " << strerror(errno) << std::endl;
      exit(1);
   }
   pollFDs = (pollfd *)realloc(pollFDs, (pollFDsNum + 1) * sizeof(pollfd));
   pollFDs[pollFDsNum].fd = clientFD;
   pollFDs[pollFDsNum].revents = 0;

   isClientFD[pollFDsNum] = true;
   pollFDs[pollFDsNum].events = POLLIN;
   pollFDsNum++;
   std::cout << "socket: " << pollFDs[i].fd << " ready to connect" << std::endl;
}

void CoreEngine::recivNClose(size_t el)
{
   // recived date is send to buffer, for now its strign
   byteRecived = recv(pollFDs[el].fd, &buffer, 1024, 0);
   buffer[byteRecived] = '\0'; 
   if (byteRecived == -1)
   {
      std::cerr << "recv() failed: " << strerror(errno) << std::endl;
      exit(1);
   }
   // this is closing socket logic, when send EOF by client EOF
   else if (byteRecived == 0)
   {
      close(pollFDs[el].fd);
      for (size_t i = el; i < pollFDsNum; i++)
      {
         if (el == (pollFDsNum - 1))
            break;
         pollFDs[i] = pollFDs[i + 1];
      }
      // shirinking pollfd 
      pollFDs = (pollfd *)realloc(pollFDs, (pollFDsNum - 1) * sizeof(pollfd));
      pollFDsNum--;
   }
   else
   {
      // response to HTTP reqest
      pollFDs[el].events = POLLIN | POLLOUT;
   }
   std::cout << buffer << std::endl; // print buffer
   std::string requestStr(buffer);
   Http response(requestStr);
   std::string responseStr = response.response();
   send(pollFDs[el].fd, responseStr.c_str(), responseStr.size(), 0); // check if string functions are ok
   
}

void CoreEngine::sendToClient(size_t el)
{
   std::string str = "Packet send sukcesfully!\n";
   int byteSend = send(pollFDs[el].fd, str.c_str(), str.size(), 0); // check if string functions are ok
   if (byteSend == -1)
   {
      std::cerr << "send() failed: " << strerror(errno) << std::endl;
      exit(1);
   }
   pollFDs[el].events = POLLIN;
   // std::cout << "client: " << pollFDs[el].fd << " ready to send" << std::endl;
}