#ifndef ADVANCEDPROGRAMMING_ASS2_CLIENT_H
#define ADVANCEDPROGRAMMING_ASS2_CLIENT_H

#include "../serverSrc/Server.h"

class Client {
public:
    /**
     * connects to the server on the given port.
     * @param port the port.
     */
    void connectToServer(const int port);
    /**
     * sends a file to the connected server.
     * @param pathToUnclassified the path to that file.
     */
    void sendFile(const std::string& pathToFile) const;
    /**
     * Receives a message from the server.
     * @return the message.
     */
    std::string receive() const;
    /**
     * Copies a string to a file.
     * @param pathToOutput the path to the file.
     * @param classified the string.
     */
    static void copyToFile(const std::string& pathToOutput, std::string& classified);
    /**
     * Disconnects from the server.
     */
    void disconnect();

private:
    //if the client is connected to a server.
    bool connected = false;

     //the client's socket.
    int socket;

    //max size of message.
    static const int bufferSize;
    /**
     * @param pathToUnclassified path to a file.
     * @return the data from the file as string.
     */
    static std::string getDataFromFile(const std::string& pathToUnclassified);

};

#endif //ADVANCEDPROGRAMMING_ASS2_CLIENT_H

