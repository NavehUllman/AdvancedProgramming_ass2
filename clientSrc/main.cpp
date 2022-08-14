#include "Client.h"
/**
 * Creates a new client, connects it to a server, and use it to classify a file (with KNN).
 */
int main(int argc, char** argv){
    int port = std::stoi(argv[1]);
    Client client;
    client.connectToServer(port);

    std::string pathToUnclassified, classified ,pathToOutput;
    for(int i = 2; i < argc; i +=2){
        pathToUnclassified = argv[i];
        pathToOutput = argv[i+1];
        std::cout << "#Sending " << pathToUnclassified << " to server." << std::endl;
        client.sendFile(pathToUnclassified);
        classified = client.receive();
        std::cout << "#Copying classified points to " << pathToOutput << std::endl;
        Client::copyToFile(pathToOutput, classified);
    }
    std::cout << "#Disconnects from server..." << std::endl;
    client.disconnect();
}

