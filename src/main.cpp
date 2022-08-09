//
// Created by Ariel Verbin on 09/08/2022.
//

#include "KNN/KNNFileClassifier.hpp"
int main(){
    KNNFileClassifier knnFileClassifier("KNN/Input/classified.csv");
    knnFileClassifier.classify(5, "KNN/Input/unclassified.csv", "KNN/Output/");
}
