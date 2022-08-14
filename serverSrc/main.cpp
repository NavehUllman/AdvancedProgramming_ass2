#include "KNN/KNNFileClassifier.hpp"
#include "Server.h"

/**
 * creates a new active server.
 */
int main(int argc, char** argv) {
    char in;
    int port = std::stoi(argv[1]);
    Server server(port, 1); //can't support more than 1 client (yet).
    while (true){
        server.waitForClients();
        std::cout << "The server has no clients. Close server? (y/n)\n"
                     "('n' means that the server will wait for a new connection)" <<std::endl;
        std::cin >> in; if(in=='y') break;
    }
    server.closeServer();
}
