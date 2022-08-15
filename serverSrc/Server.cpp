#include "Server.h"
#include "KNN/KNNFileClassifier.hpp"
#include "KNN/DistanceCalcs/EuclideanDistance.hpp"

const int Server::bufferSize = 128;

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
        close(sock);
        exit(1);
    }
}

void Server::listen() const {
    if (::listen(this->sock, this->maxClients) < 0) {
        perror("Error listening to a socket");
        close(sock);
        exit(1);
    }
}

int Server::waitForClient() {
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
    std::cout << "#New client (" << clientSock << ") connected." << std::endl;
    return clientSock;
}

void Server::removeClient(int clientSock) {
    std::cout << "#Disconnects from client (" << clientSock << ")..." << std::endl;
    close(clientSock);
    this->numClients--;
}

void Server::communicate(int clientSock) {
    bool stillConnected = true;
    while (stillConnected) {
        std::string received = this->receive(clientSock); //receive data from client.
        if (received == "<client_closed>") stillConnected = false; //client closed connection.
        else {
            std::vector<Point> unclassified = Point::toPoints(received, '|');

            KNNFileClassifier knnFileClassifier(this->databaseFile);
            DistanceCalculator *dc = new EuclideanDistance();
            std::cout << "#Classifying unclassified points... (sent from client: " << clientSock << ")" << std::endl;
            std::vector<Flower> classified = knnFileClassifier.classify(5, unclassified, dc);
            delete dc;
            std::string toSend = Flower::toFileFormat(classified);
            //send classified to client, remove client if error occurred.
            if(!this->send(clientSock, toSend)) this->removeClient(clientSock);
        }
    }
    this->removeClient(clientSock);
}

/**
 * This method takes a fragmented message and returns the defragged version.
 * for example: "<This me><ssage righ><t here>$" TO: "This message right here"
 * @param raw the raw message.
 * @return the new one.
 */
std::string defrag(std::string &raw) {
    raw.erase(remove(raw.begin(), raw.end(), '<'), raw.end());
    raw.erase(remove(raw.begin(), raw.end(), '>'), raw.end());
    raw.erase(remove(raw.begin(), raw.end(), '$'), raw.end());
    return raw;
}

std::string Server::receive(int clientSock) {
    std::string rawMessage;
    char buffer[Server::bufferSize+1] = {0};

    while (rawMessage.find('$') == std::string::npos) {
        int read_bytes = (int) ::recv(clientSock, buffer, sizeof(buffer), 0); //receive data from client.

        if (read_bytes == 0) { return "<client_closed>"; }
        else if (read_bytes < 0) { std::cout << "Error reading bytes." << std::endl;
            return "<client_closed>";
        }
        buffer[Server::bufferSize] = '\0';
        rawMessage.append(buffer);
    }
    return defrag(rawMessage);
}

bool Server::send(int clientSock, std::string &classified) {
    int index = 0;
    char buffer[Server::bufferSize];

    while (index + Server::bufferSize - 3 <= classified.size()) { //send in pieces of 128 bytes.
        strcpy(buffer, ("<" + classified.substr(index, Server::bufferSize - 2) + ">").c_str());
        index += Server::bufferSize - 2;
        int sent_bytes = (int) ::send(clientSock, buffer, sizeof(buffer), 0);

        if (sent_bytes < 0) { std::cout << "error in sending bytes" << std::endl;
            return false;
        }
    }
    //rest of message (will be smaller than 128 bytes):
    strcpy(buffer, ("<" + classified.substr(index, classified.size()) + ">$").c_str());
    int sent_bytes = (int) ::send(clientSock, buffer, sizeof(buffer), 0);

    if (sent_bytes < 0) { std::cout << "error in sending bytes" << std::endl;
        return false;
    }

    return true;
}

void Server::closeServer() const {
    close(sock);
}


