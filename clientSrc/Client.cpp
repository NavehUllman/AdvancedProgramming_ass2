#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include "Client.h"
#include <fstream>
#include <utility>

const int Client::bufferSize = 128;


void Client::connectToServer(const int port) {
    if (connected) {
        std::cout << "Already connected to a server" << std::endl;
        return;
    }
    const char *ip_address = "127.0.0.1"; //connection between the device to itself;
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);

    if (socket < 0) { perror("Error creating socket");
        exit(1);
    }
    struct sockaddr_in sin{};
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port);

    if (connect(socket, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("Error connecting to server"); exit(1);
    }
    this->connected = true;
}

bool Client::sendFile(const std::string &pathToUnclassified) const {
    std::string unclassified = Client::getDataFromFile(pathToUnclassified);
    int index = 0;
    char buffer[Client::bufferSize];

    while (index + Client::bufferSize - 3 <= unclassified.size()) { //send in pieces of 128 bytes.
        strcpy(buffer, ("<" + unclassified.substr(index, Client::bufferSize - 2) + ">").c_str());
        index += Client::bufferSize - 2;
        int sent_bytes = (int) ::send(socket, buffer, sizeof(buffer), 0);
        if (sent_bytes < 0) {
            std::cout << "error in sending bytes" << std::endl;
            return false;
        }
    }
    //rest of message (will be smaller than 128 bytes):
    strcpy(buffer, ("<" + unclassified.substr(index, unclassified.size()) + ">$").c_str());
    int sent_bytes = (int) ::send(socket, buffer, sizeof(buffer), 0);

    if (sent_bytes < 0) {
        std::cout << "error in sending bytes" << std::endl;
        return false;
    }
    return true;
}
/**
 * This method takes a fragmented message and returns the defragged version.
 * for example: <This me><ssage righ><t here& TO: This message right here
 * @param raw the raw message.
 * @return the new one.
 */
std::string defrag(std::string &raw) {
    raw.erase(remove(raw.begin(), raw.end(), '<'), raw.end());
    raw.erase(remove(raw.begin(), raw.end(), '>'), raw.end());
    raw.erase(remove(raw.begin(), raw.end(), '$'), raw.end());
    return raw;
}

std::string Client::receive() const {
    std::string rawMessage;
    char buffer[Client::bufferSize] = {0};
    while (rawMessage.find('$') == std::string::npos) {
        int read_bytes = (int) ::recv(this->socket, buffer, sizeof(buffer), 0); //receive data from client.
        if (read_bytes == 0) {
            return "<server_closed>";
        } else if (read_bytes < 0) {
            std::cout << "Error reading bytes." << std::endl;
            return "<server_closed>";
        }
        rawMessage.append(buffer);
    }
    return std::move(defrag(rawMessage));
}

void Client::disconnect() {
    close(socket);
    this->connected = false;
}

std::string Client::getDataFromFile(const std::string &pathToUnclassified) {
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

void Client::copyToFile(const std::string &pathToOutput, std::string &classified) {
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