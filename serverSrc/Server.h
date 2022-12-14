
#ifndef ADVANCEDPROGRAMMING_ASS2_SERVER_H
#define ADVANCEDPROGRAMMING_ASS2_SERVER_H

#include <iostream>
#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include "../clientSrc/Client.h"
#include "KNN/Point.hpp"
#include "KNN/Flowers/Flower.hpp"

/**
 * Server class is basically a wrapper class to socket.h, making it easier to understand and more OOP.
 */
class Server {
public:
    /**
    * Creates a new socket for the server and binds it to a socked address (local ip and given port).
    * @param port The port to which the server socket will be bound.
    * @param maxClients maximum clients possible for the server.
    */
    Server(int port, int maxClients);
    /**
     * Waits for clients (meanwhile only one) to connect, and accept them.
     * @return the client sock fd of the connected client.
     */
    int waitForClient();
    /**
     * Receives data from a client.
     * @param clientSock the client socket fd.
     * @return the sent data, as string.
     */
    std::string receive(int clientSock);
    /**
     * Sends data for a client.
     * @param clientSock the client socket fd.
     * @param toSend the message to be sent.
     * @return true if it was successful, false if an error occurred.
     */
    bool send(int clientSock, std::string &toSend);
    /**
     * Receives data from client, classifies it according to the KNN algorithm, and sends it back.
     * @param clientSock socket fd between server and client.
     */
    void communicate(int clientSock);
    /**
     * Closes the server's socket.
     */
    void closeServer() const;

private:
    /**
     * Binds the new socket to a socket address.
     * @param port the port (as part of the socket address).
     */
    void bind(const int port);
    /**
     * Announce willingness to accept connections.
     */
    void listen() const;

    //max clients possible.
    const int maxClients;

    //path to the database file.
    const std::string databaseFile = "KNN/Database/classified.csv";

    //address of socket.
    struct sockaddr_in socketAddress;

    //Server socket fd, current amount fo clients.
    int sock, numClients;

    //max size of message.
    static const int bufferSize;
    /**
    * Closes and remove the given client.
    * @param clientSock socket fd to close.
    */
    void removeClient(int clientSock);

};


#endif //ADVANCEDPROGRAMMING_ASS2_SERVER_H
