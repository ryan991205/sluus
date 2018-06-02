#include <string.h>     // for strlen()
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "comLibs/Auxiliary.h"
#include "comLibs/CreateTCPClientSocket.h"
#include "Communicator.h"

Communicator::Communicator(int port, const char* serverIP)
{
    sock = CreateTCPClientSocket(serverIP, port);
}

Communicator::~Communicator()
{
    close(sock);
}

std::string Communicator::Transmit(const std::string message)
{
    Send(message);
    return Recieve();
}

void Communicator::Send(const std::string message)
{
    size_t size = message.length();
    if(message.length() == 0)
    {
        throw std::logic_error("message.Lenght != 0");
    }

    send(sock, message.c_str(), size, 0);
}


std::string Communicator::Recieve()
{
    size_t bytesReceived = recv(sock,buffer,RCVBUFSIZE,0);
    if(bytesReceived == 0)
    {
        return NULL;
    }

    return std::string(remove_nl(buffer));
}