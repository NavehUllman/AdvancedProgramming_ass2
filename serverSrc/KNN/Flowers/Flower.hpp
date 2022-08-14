#ifndef ADVANCEDPROGRAMMING_FLOWER_H
#define ADVANCEDPROGRAMMING_FLOWER_H

#include <iostream>
#include <string> // for string class
#include"../Point.hpp"

//A Flower - a Point + type
class Flower {
//the flower's data
        Point point;
        //the flower's type
        std::string type;
    public:
    /**
     * Constructor
     * @param point set of values.
     * @param type of the flower.
     */
    Flower(const Point &point, std::string type);
    /**
     * Constructor using a string representation.
     * @param str in form : "value,value,value,...,value,type".
     * @param numOfValues the number of values for the point.
     */
    Flower(const std::string &str);
    /**
     * @brief Get the Point object
     * 
     * @return Point 
     */
    Point& getPoint();

    /**
     * @brief 
     * @return std::string
     */
    std::string& getType();
    /**
     * @param vec vector of flowers.
     * @return the string that represent that flowers in a file format (only flower-type, seperated by \n).
     */
    static std::string toFileFormat(std::vector<Flower> vec);

    friend std::ostream& operator<<(std::ostream& os, Flower& a);

};
#endif //ADVANCEDPROGRAMMING_FLOWER_H
