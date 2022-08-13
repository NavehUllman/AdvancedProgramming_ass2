
#include "KNN/KNNFileClassifier.hpp"
int main(){
    KNNFileClassifier knnFileClassifier("serverSrc/KNN/Input/classified.csv");
    knnFileClassifier.classify(5, "serverSrc/KNN/Input/unclassified.csv", "serverSrc/KNN/Output/");
}
