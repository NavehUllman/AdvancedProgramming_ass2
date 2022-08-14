#include "Server.h"
#include "KNN/KNNFileClassifier.hpp"
#include "KNN/DistanceCalcs/EuclideanDistance.hpp"
Server::Server(int port, int maxClients) : port(port), maxClients(maxClients) {
    this->sock = socket(AF_INET, SOCK_STREAM, 0); //create a new socket - IPv4, TCP.
    if (this->sock < 0) {
        perror("Error creating socket");
        exit(1);
    }

    struct sockaddr_in sin{}; //new struct to save the socket address.

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET; //family (TCP)
    sin.sin_addr.s_addr = INADDR_ANY;//IP - on our local machine.
    sin.sin_port = htons(this->port); //selected port.

    if (bind(this->sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) { //bind new socket to selected socked address.
        perror("Error binding socket to given socket address");
        exit(1);
    }

    if (listen(this->sock, this->maxClients) < 0) {
        perror("Error listening to a socket");
        exit(1);
    }

}
Server& Server::getServerFrom(int port) {
    for(auto &p: Server::activeServers) {
        if(port == p.port) {
            return p;
        }
    }
    throw std::invalid_argument("No active servers on given port.");
}

bool Server::waitForClients() {
    if(this->numClients >= this->maxClients) {
        return false;
    }
    struct sockaddr_in client_sin{}; //new socked address of new client.
    unsigned int addr_len = sizeof(client_sin);
    //wait until a client connects, create a new socket.
    int clientSock = accept(this->sock,  (struct sockaddr *) &client_sin,  &addr_len);
    if (clientSock < 0) {
        perror("Error accepting client");
        exit(1);
    }
    this->numClients++;
    //In the future it will call this function via a new thread, the main thread will keep accepting clients.
    this->communicate(clientSock);
    return true;
}
void Server::removeClient(int clientSock) {
    close(clientSock);
    this->numClients--;
}

void Server::communicate(int clientSock) {
    std::vector<Point> unclassified = this->receive(clientSock);
    //Classifies:
    KNNFileClassifier knnFileClassifier("Input/classified.csv");
    DistanceCalculator *dc = new EuclideanDistance();
    std::vector<Flower> classidied = knnFileClassifier.classify(5,unclassified,dc);
    this->send(clientSock, classidied);
    delete dc;
}

std::vector<Point> Server::receive(int clientSock) {
    std::vector<Point> unclassified;
    char buffer[20] = {0};
    int read_bytes = (int)recv(clientSock, buffer, sizeof(buffer), 0); //receive data from client.
    if(read_bytes == 0) { //connection closed in the middle, aborting and closing.
        this->removeClient(clientSock);
    }
        while(!strcmp(buffer,"<END_INPUT>")) {
        unclassified.emplace_back(Point(buffer)); //add new flower to the vector.
        read_bytes = (int) recv(clientSock, buffer, sizeof(buffer), 0); //receive data from client.
        if(read_bytes == 0) {
            std::cout << "Connection closed in the middle, aborting and closing." << std::endl;
            this->removeClient(clientSock);
        }
    }
    return unclassified;
}

void Server::send(int clientSock, std::vector<Flower> &classified) {
    char buffer[20]={0};
    for(auto &flower : classified) {
        std::size_t length = flower.getType().copy(buffer,flower.getType().size(),0);
        buffer[length]='\0';
        int sent_bytes = (int)::send(clientSock, buffer, sizeof(buffer), 0); //send data for client.
        if (sent_bytes < 0) {
            perror("Error sending to client");
            close(clientSock);
            exit(1);
        }
    }
    char endMessage[] = "<END_OUTPUT>";
    int sentBytes = (int)::send(clientSock, endMessage, sizeof(endMessage), 0); //send data for client.
    if (sentBytes < 0) {
        perror("Error sending to client");
        close(clientSock);
        exit(1);
    }
}
