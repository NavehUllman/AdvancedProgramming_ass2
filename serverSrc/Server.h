
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
     * @param port the port to which the server is connected.
     * @return a reference of an instance of the active server in the given port. It might throw an exception and
     * terminate the program if the port isn't bound to a server.
     */
    static Server& getServerFrom(int port);
    /**
     * Adds the first client in queue that tries to connect to this server.
     * @return true if the connection was successful, false if amount of clients already on full capacity.
     */
    bool waitForClients();
    /**
     * Receives data from a client.
     * @param client the client.
     */
    std::vector<Point> receive(int clientSock);
    /**
     * Sends data for a client.
     * @param client the client.
     */
    void send(int clientSock, std::vector<Flower> &classified);
    /**
     * Send and receive data from client until end of connection.
     * @param clientSock socket fd between server and client.
     */
    void communicate(int clientSock);

private:
    //Port to which the server socket is bound to, max clients possible.
    const int port, maxClients;
    //Server socket fd, current amount fo clients.
    int sock, numClients;
    //list of servers in process (so we know to which ports the clients can connect to).
    static std::vector<Server> activeServers;
    /**
    * Closes and remove the given client.
    * @param clientSock socket fd to close.
    */
    void removeClient(int clientSock);

};


#endif //ADVANCEDPROGRAMMING_ASS2_SERVER_H
