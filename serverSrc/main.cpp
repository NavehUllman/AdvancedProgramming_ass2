#include "KNN/KNNFileClassifier.hpp"
#include "Server.h"

/**
 * creates a new active server.
 */
int main(int argc, char **argv) {
    int port = std::stoi(argv[1]), numOfConnections = 0;
    if (argc == 3) numOfConnections = std::stoi(argv[2]); //optional: specified number of connection until closure.

    Server server(port, 1); //can't support more than 1 client (yet).
    for (int i = 0; numOfConnections == 0 || i < numOfConnections; i++) {
        server.waitForClients();
        std::cout << "#Finished connection " << i + 1 << "." << std::endl;
    }
    server.closeServer();
}
