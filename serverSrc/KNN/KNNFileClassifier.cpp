#include <iostream>
#include "Point.hpp"
#include "Flowers/Flower.hpp"
#include "KNNClassifier.hpp"
#include "DistanceCalcs/ManhattanDistance.hpp"
#include "DistanceCalcs/EuclideanDistance.hpp"
#include "DistanceCalcs/ChebyshevDistance.hpp"
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

std::vector<Point> KNNFileClassifier::getPointsFromFile(const std::string &fileName) {
    std::vector<Point> l;
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
        l.emplace_back(Point(line));
    }
    input.close();
    return l;
}

void KNNFileClassifier::copyToFile(const std::string &fileName, std::vector<Flower> flowers) {
    ofstream output;
    output.open(fileName);
    for (auto &flower: flowers) {
        output << flower.getType() << endl;
    }
    output.close();
}

std::vector<Flower> KNNFileClassifier::classifyAll(DistanceCalculator &dc, KNNClassifier &knn,
                                                   std::vector<Point> &unclassifiedPoints, int k) {
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
void KNNFileClassifier::classify(int k, const std::string& pathToUnclassified, const std::string& pathToOutputFolder) const {
    std::vector<Flower> flowers = getDataFromFile(this->pathToClassified); //fill "flowers" with classifieds.
    std::vector<Point> unclassifiedPoints = getPointsFromFile(pathToUnclassified);

    KNNClassifier knnClassifier = KNNClassifier(flowers); //new KNNClassifier.
    auto *euclideanDistance = new EuclideanDistance();
    auto *chebyshevDistance = new ChebyshevDistance();
    auto *manhattanDistance = new ManhattanDistance();

//EuclideanDistance

    std::vector<Flower> classifiedPoints = classifyAll(*euclideanDistance,
                                                       knnClassifier, unclassifiedPoints, k);
    //copy the flowers to the file.
    copyToFile(pathToOutputFolder+"/euclidean_output.csv", classifiedPoints);

//ManhattanDistance

    classifiedPoints = classifyAll(*manhattanDistance,
                                   knnClassifier, unclassifiedPoints, k);
    //copy the flowers to the file
    copyToFile(pathToOutputFolder+"/manhattan_output.csv", classifiedPoints);

//ChebyshevDistance

    classifiedPoints = classifyAll(*chebyshevDistance,
                                   knnClassifier, unclassifiedPoints, k);
    //copy the flowers to the file.
    copyToFile(pathToOutputFolder+"/chebyshev_output.csv", classifiedPoints);

    delete euclideanDistance;
    delete manhattanDistance;
    delete chebyshevDistance;
}