#include "KNNClassifier.hpp"

//find the most popular type
KNNClassifier::KNNClassifier(std::vector<Flower> &data) : flowers(data) {
}

std::string KNNClassifier::findMajorityType(std::vector<Flower> &flowers) {
    int array[3] = {0, 0, 0};
    for (auto &flower: flowers) {
        if (flower.getType() == "Iris-setosa") {
            array[0]++;
        } else if (flower.getType() == "Iris-versicolor") {
            array[1]++;
        } else {
            array[2]++;
        }
    }
    //finally, find:
    if (array[0] > array[1]) {
        if (array[0] > array[2]) {
            return "Iris-setosa";
        } else
            return "Iris-virginica";
    } else if (array[1] > array[2]) {
        return "Iris-versicolor";
    } else
        return "Iris-virginica";
}

void KNNClassifier::selection(const Point &point, int k, DistanceCalculator &dc) {
    //selection method.
    int minIndex;
    double minValue, curValue;
    for(int i = 0; i < k; i ++) {
        minValue = dc.calculate(point, flowers[i].getPoint());
        for(int j = i+1; j< flowers.size(); j++){
            curValue = dc.calculate(point,flowers[j].getPoint());
            if(curValue < minValue) {
                minIndex = j;
                minValue = curValue;
                Flower temp = flowers[i];
                flowers[i] = flowers[minIndex];
                flowers[minIndex] = temp;
            }
        }
    }

}

//get the most popular type
std::string KNNClassifier::classify(const Point &point, int k, DistanceCalculator &dc) {

    this->selection(point, k , dc);

    std::vector<Flower> vec;
    for (int i = 0; i < k; i++) vec.push_back(flowers[i]);

    //now we have a vector of the flowers sort by their distance from our point.
    //we need now to find the most popular type
    return findMajorityType(vec);

}