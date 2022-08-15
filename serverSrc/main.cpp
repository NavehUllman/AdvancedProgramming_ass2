#include "KNN/KNNFileClassifier.hpp"
#include "Server.h"

/**
 * creates a new active server.
 */
int main(int argc, char **argv) {
    int port = std::stoi(argv[1]), numOfConnections = 0;
    if (argc == 3) numOfConnections = std::stoi(argv[2]); //optional: specified number of connection until closure.

    Server server(port, 1); //can't support more than 1 client (yet).
    int clientSock;
    for (int i = 0; numOfConnections == 0 || i < numOfConnections; i++) {
        clientSock = server.waitForClient();
        server.communicate(clientSock); //via a new thread in the future.
        std::cout << "#Finished connection " << i + 1 << "." << std::endl;
    }
    server.closeServer();
}
