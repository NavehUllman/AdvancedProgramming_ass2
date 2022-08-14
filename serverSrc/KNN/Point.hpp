#ifndef ADVANCEDPROGRAMMING_POINT_H
#define ADVANCEDPROGRAMMING_POINT_H

#include <iostream>
#include <vector>

//A four-dimensional point.
class Point {
    private:
        std::vector<double> data;
    public:
  /**
   * @brief Construct a new Point object
   * 
   * @param d 
   */
    explicit Point(std::vector<double> d);
    /**
     * @brief Construct a new Point object
     * 
     * @param str 
     */
    explicit Point(const std::string& str);

    /**
     * @param i, the index of the value.
     * @return the value in the i-th place.
     **/
    double get(int i) const;
    /**
     * @return the number of double values.
     */
    int getLength() const;
    /**
     * @param buffer list of points (represented by strings), seperated by a char.
     * @param c the char.
     * @return a vector of those points.
     */
    static std::vector<Point> toPoints(std::string& buffer, char c);


    /**
     * Overloading the << operator.
     * @param os the output stream.
     * @param a the point,
     * @return the output stream with the addition of Point.
     */
    friend std::ostream& operator<<(std::ostream& os, const Point& a);
};

#endif //ADVANCEDPROGRAMMING_POINT_H
