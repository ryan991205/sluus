#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include<stdexcept>
#include<string>

#define RCVBUFSIZE 64   /* Size of receive buffer */

class Communicator
{
public:
    Communicator(int port, const char* serverIP = "127.0.0.1");
    ~Communicator();


    std::string Transmit(const std::string message);      //sends and recieves

private:
    int  sock;
    char buffer[RCVBUFSIZE];

    void Send(const std::string message);
    std::string Recieve();

    bool ValidMessage(const char message[]);
};


#endif

