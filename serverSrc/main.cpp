
#include "KNN/KNNFileClassifier.hpp"
#include "Server.h"
int main(){
    Server server(5555,1);
    server.waitForClients();
}
