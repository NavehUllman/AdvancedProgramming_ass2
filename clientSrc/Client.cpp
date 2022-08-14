#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "Client.h"
#include <vector>
#include <fstream>

const int Client::bufferSize = 512;


void Client::connectToServer(const int port) {
    if (connected) {
        std::cout << "Already connected to a server" << std::endl;
        return;
    }
    const char *ip_address = "127.0.0.1"; //connection between the device to itself;
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0) {
        perror("Error creating socket");
        exit(1);
    }
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port);

    if (connect(socket, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("Error connecting to server");
    }
    this->connected = true;
}

void Client::sendFile(std::string pathToUnclassified) {
    std::string unclassified = Client::getDataFromFile(pathToUnclassified);

    char buffer[Client::bufferSize];
    if (unclassified.size() >= Client::bufferSize) {
        std::cout << "Unclassified file is too big." << std::endl;
    }

    strcpy(buffer, unclassified.c_str());
    int sent_bytes = ::send(socket, buffer, sizeof(buffer), 0);

    if (sent_bytes < 0) {
        std::cout << "error in sending bytes" << std::endl;
    }
}

std::string Client::receive() {
    char buffer[Client::bufferSize];
    int read_bytes = recv(socket, buffer, sizeof(buffer), 0);
    if (read_bytes == 0) {
        std::cout << "Connection was closed on server" << std::endl;
        close(socket);
        exit(1);
    } else if (read_bytes < 0) {
        std::cout << "Error in reading bytes" << std::endl;
        close(socket);
        exit(1);
    }

    std::string classified = buffer;
    return std::move(classified);
}

void Client::disconnect() {
    close(socket);
    this->connected = false;
}

std::string Client::getDataFromFile(std::string pathToUnclassified) {
    std::string data, line;

    std::ifstream input;
    input.open(pathToUnclassified);

    if (input.fail()) {
        std::cout << "Could not open " << pathToUnclassified << std::endl;
        input.close();
        exit(1);
    }

    while (!input.eof()) {
        getline(input, line);
        if (std::equal(line.begin(), line.end(), "")) continue; // avoid an empty line.
        data.append(line + "|");
    }

    input.close();
    return data;
}

void Client::copyToFile(std::string pathToOutput, std::string &classified) {
    std::ofstream output;
    output.open(pathToOutput);
    if (output.fail()) {
        std::cout << "Could not open " << pathToOutput << std::endl;
        output.close();
        throw std::exception();
    }
    output << classified;
    output.close();
}