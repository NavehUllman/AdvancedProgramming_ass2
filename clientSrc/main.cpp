#include "Client.h"
/**
 * Creates a new client, connects it to a server, and use it to classify a file (with KNN).
 */
int main(int argc, char** argv){
    int port = std::stoi(argv[1]);
    std::cout << "------------Client-------------\n";
    Client client;
    client.connectToServer(port);

    std::string pathToUnclassified, classified ,pathToOutput;
    for(int i = 2; i < argc; i +=2){
        pathToUnclassified = argv[i]; //input
        pathToOutput = argv[i+1]; //output
        //send:
        std::cout << "#Sending " << pathToUnclassified << " to server." << std::endl;
        if(!client.sendFile(pathToUnclassified)) break; //send file and break if error occurred.
        //receive back:
        classified = client.receive();
        if(classified == "<server_closed>") {std::cout << "Server closed, Abort.\n"; break;}
        //copy to file:
        std::cout << "#Copying classified points to " << pathToOutput << std::endl;
        if(!Client::copyToFile(pathToOutput, classified)) {
            std::cout << "Error in copying to file: " << pathToOutput << "." << std::endl;
            break;
        }
        std::cout << std::endl;
    }
    std::cout << "#Disconnects from server..." << std::endl;
    client.disconnect();
    std::cout << "-------------------------------" << std::endl;
}

