#include <iostream>
#include "Point.hpp"
#include "Flowers/Flower.hpp"
#include "KNNClassifier.hpp"
#include "DistanceCalcs/DistanceCalculator.hpp"
#include <fstream>
#include <utility>
#include <vector>
#include "KNNFileClassifier.hpp"

using namespace std;

KNNFileClassifier::KNNFileClassifier(std::string pathToClassified) : pathToClassified(std::move(pathToClassified)){}

std::vector<Flower> KNNFileClassifier::getDataFromFile(const std::string &fileName) {
    std::vector<Flower> l;
    string line;
    ifstream input;
    input.open(fileName);
    if (input.fail()) {
        cout << "Could not open " << fileName << endl;
        input.close();
        throw std::exception();
    }
    while (!input.eof()) {
        getline(input, line);
        if (std::equal(line.begin(), line.end(), "")) continue; // avoid an empty line.
        l.emplace_back(Flower(line));
    }
    input.close();
    return l;
}

//void KNNFileClassifier::copyToFile(const std::string &fileName, std::vector<Flower> flowers) {
//    ofstream output;
//    output.open(fileName);
//    for (auto &flower: flowers) {
//        output << flower.getType() << endl;
//    }
//    output.close();
//}

std::vector<Flower> KNNFileClassifier::classifyAll(DistanceCalculator &dc, KNNClassifier &knn,
                                                   const std::vector<Point> &unclassifiedPoints, int k) {
    std::vector<Flower> classifiedPoints; //to be filled with the classified points.

    for (auto &curPoint: unclassifiedPoints) {
        //take the current point.
        classifiedPoints.emplace_back(Flower(curPoint, //classify it and create a new flower.
                                             knn.classify(curPoint, k, dc)));
    }
    return classifiedPoints;
}

/**
 * main method.
 */
std::vector<Flower> KNNFileClassifier::classify(int k, const std::vector<Point> &unclassifiedPoints,
                                                DistanceCalculator *dc) const {
    std::vector<Flower> flowers = getDataFromFile(this->pathToClassified); //fill "flowers" with classifieds.

    KNNClassifier knnClassifier = KNNClassifier(flowers); //new KNNClassifier.

    return classifyAll(*dc,knnClassifier, unclassifiedPoints, k);

}