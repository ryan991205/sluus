#include "Communicator.h"

#include <string.h>     // for strlen()
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "Auxiliary.h"
#include "CreateTCPClientSocket.h"


Communicator::Communicator(int port, const char* serverIP)
{
    sock = CreateTCPClientSocket(serverIP, port);
}

Communicator::~Communicator()
{
    close(sock);
}

std::string Communicator::Transmit(std::string message)
{
    std::lock_guard<std::mutex> lock(mutex);

    Send(message);
    return Recieve();
}

void Communicator::Send(std::string message)
{
    size_t size = message.length();
    if(message.length() == 0)
    {
        throw std::logic_error("Communicator::Send(): message.Lenght != 0");
    }

    send(sock, message.c_str(), size, 0);
}


std::string Communicator::Recieve()
{
    size_t bytesReceived = recv(sock,buffer,RCVBUFSIZE,0);
    if(bytesReceived == 0)
    {
        return "Error: recieved message length == 0";
    }

    std::string rec = std::string(remove_nl(buffer));
    rec = rec.substr(0, rec.find(";") + 1);

    return rec;
}