#include "CoreEngine.hpp"

CoreEngine::client& CoreEngine::getClientByFD(int socketFD)
{
    for(size_t i = 0; i < this->clientVec.size(); i++)
        if(clientVec[i].FD == socketFD)
            return clientVec[i];
    std::stringstream ss;
    ss << "Client not found for socket FD: " << socketFD;
    throw std::runtime_error(ss.str());
}