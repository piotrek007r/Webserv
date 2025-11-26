// Copyright [2025] <Piotr Ruszkiewicz> <pruszkie@student.42warsaw.pl>

#include "CoreEngine.hpp"
#include "../http/Http.hpp"

void CoreEngine::setConnection(size_t i)
{
   socklen_t addrLen = sizeof(sockaddr_storage);
   client client;
   memset(&client.clientSockaddr, 0, sizeof(sockaddr_in));

   clientVec.push_back(client);
   client.FD = accept(pollFDs[i].fd, (struct sockaddr *)&client.clientSockaddr, &addrLen);
   if (client.FD == -1)
   {
      std::cerr << "accept() failed: " << strerror(errno) << std::endl;
      exit(1);
   }
   pollFDs = (pollfd *)realloc(pollFDs, (pollFDsNum + 1) * sizeof(pollfd));
   pollFDs[pollFDsNum].fd = client.FD;
   pollFDs[pollFDsNum].revents = 0;

   isClientFD[pollFDsNum] = true;
   pollFDs[pollFDsNum].events = POLLIN;
   pollFDsNum++;
   this->clientVec.push_back(client);
   std::cout << "socket: " << pollFDs[i].fd << " ready to connect" << std::endl;
}

void CoreEngine::recivNClose(size_t el)
{
   // recived date is send to buffer, for now its strign
   client &client = this->getClientByFD(pollFDs[el].fd);
   client.byteRecived = recv(pollFDs[el].fd, client.buffer, 1024, 0);
   client.buffer[client.byteRecived] = '\0';
   if (client.byteRecived == -1)
   {
      std::cerr << "recv() failed: " << strerror(errno) << std::endl;
      return;
   }
   // this is closing socket logic, when send EOF by client EOF
   else if (client.byteRecived == 0)
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
      std::cout << "--->buffer: " << client.buffer << std::endl; // print buffer
      pollFDs[el].events = POLLOUT;
   }
}

void CoreEngine::sendToClient(size_t el)
{
   client &client = this->getClientByFD(pollFDs[el].fd);
   std::string requestStr(client.buffer);
   Http response(requestStr);
   std::string responseStr = response.response();
   std::string str = "Packet send sukcesfully!\n";
   int byteSend = send(pollFDs[el].fd, responseStr.c_str(), responseStr.size(), 0); // check if string functions are ok
   if (byteSend == -1)
   {
      std::cerr << "send() failed: " << strerror(errno) << std::endl;
      return;
   }
   pollFDs[el].events = POLLIN;
   // std::cout << "client: " << pollFDs[el].fd << " ready to send" << std::endl;
}