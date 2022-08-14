#include "Server.h"
#include "KNN/KNNFileClassifier.hpp"
#include "KNN/DistanceCalcs/EuclideanDistance.hpp"

const int Server::bufferSize = 512;

Server::Server(int port, int maxClients) : socketAddress{}, maxClients(maxClients) {
    this->sock = socket(AF_INET, SOCK_STREAM, 0); //create a new socket - IPv4, TCP.
    if (this->sock < 0) {
        perror("Error creating socket");
        exit(1);
    }
    this->bind(port);
    this->listen();

    this->numClients = 0;
}

void Server::bind(const int port) {
    memset(&socketAddress, 0, sizeof(this->socketAddress));
    this->socketAddress.sin_family = AF_INET; //family (TCP)
    this->socketAddress.sin_addr.s_addr = INADDR_ANY;//IP - on our local machine.
    this->socketAddress.sin_port = htons(port); //selected port.

    //bind new socket to selected socked address.
    if (::bind(this->sock, (struct sockaddr *) &socketAddress, sizeof(socketAddress)) < 0) {
        perror("Error binding socket to given socket address");
        exit(1);
    }
}

void Server::listen() {
    if (::listen(this->sock, this->maxClients) < 0) {
        perror("Error listening to a socket");
        exit(1);
    }
}

bool Server::waitForClients() {
    if (this->numClients >= this->maxClients) {
        std::cout << "Cannot wait for more clients: at max capacity." << std::endl;
        return false;
    }
    struct sockaddr_in client_sin{}; //new socked address of new client.
    unsigned int addr_len = sizeof(client_sin);
    //wait until a client connects. then, create a new socket.
    int clientSock = accept(this->sock, (struct sockaddr *) &client_sin, &addr_len);
    if (clientSock < 0) {
        perror("Error accepting client");
        close(sock);
        exit(1);
    }
    this->numClients++;
    std::cout <<"#New client (" << clientSock << ") connected." <<std::endl;
    //In the future it will call this function via a new thread, the main thread will keep accepting clients.
    this->classifyKNN(clientSock);
    return true;
}

void Server::removeClient(int clientSock) {
    std::cout << "#Disconnects from client (" << clientSock << ")..." << std::endl;
    close(clientSock);
    this->numClients--;
}

void Server::classifyKNN(int clientSock) {
    bool stillConnected = true;
    while(stillConnected) {
        std::string received = this->receive(clientSock);
        if (received.compare("<client_closed>") == 0) stillConnected = false; //client closed connection.
        else {
            std::vector<Point> unclassified = Point::toPoints(received, '|');

            KNNFileClassifier knnFileClassifier("KNN/Database/classified.csv");
            DistanceCalculator *dc = new EuclideanDistance();
            std::cout << "#Classifying unclassified points (sent from client: " << clientSock <<")"<<std::endl;
            std::vector<Flower> classified = knnFileClassifier.classify(5, unclassified, dc);
            delete dc;
            std::string toSend = Flower::toFileFormat(classified);
            this->send(clientSock, toSend);
        }
    }
    this->removeClient(clientSock);
}

std::string Server::receive(int clientSock) {
    char buffer[Server::bufferSize];

    int read_bytes = (int) recv(clientSock, buffer, sizeof(buffer), 0); //receive data from client.
    if (read_bytes == 0) {
        return "<client_closed>";
    } else if (read_bytes < 0) {
        std::cout << "Error reading bytes." << std::endl;
        this->removeClient(clientSock);
        close(sock);
        exit(1);
    }
    std::string data = buffer;
    return data;
}

void Server::send(int clientSock, std::string &classified) {
    char buffer[bufferSize];

    if (classified.size() >= bufferSize) {
        std::cout << "Classified file is too big." << std::endl;
        this->removeClient(clientSock);
        close(sock);
        exit(1);
    }

    strcpy(buffer, classified.c_str());

    int sent_bytes = ::send(clientSock, buffer, sizeof(buffer), 0);

    if (sent_bytes < 0) {
        std::cout << "error in sending bytes" << std::endl;
        this->removeClient(clientSock);
        close(sock);
        exit(1);
    }
}

void Server::closeServer() {
    close(sock);
}


