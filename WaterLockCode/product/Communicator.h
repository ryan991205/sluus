#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <stdexcept>
#include <string>
#include <mutex>

#define RCVBUFSIZE 64   /* Size of receive buffer */


class Communicator
{
public:
    Communicator(int port, const char* serverIP = "127.0.0.1");
    ~Communicator();

    std::string Transmit(std::string message);      //sends and recieves

private:
    int  sock;
    char buffer[RCVBUFSIZE];
    std::mutex mutex;

    void Send(std::string message);
    std::string Recieve();

    bool ValidMessage(const char message[]);

    // private copy constructor and assignment operator to prevent making copies
    Communicator(const Communicator&) { /* do nothing */ };
    Communicator& operator= (const Communicator&) { return *this; };
};

#endif

