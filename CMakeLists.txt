cmake_minimum_required(VERSION 3.20)
project(AdvancedProgramming_ass2)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/KNN)
include_directories(src/KNN/DistanceCalcs)
include_directories(src/KNN/Flowers)

add_executable(AdvancedProgramming_ass2
        src/main.cpp
        src/KNN/DistanceCalcs/ChebyshevDistance.cpp
        src/KNN/DistanceCalcs/ChebyshevDistance.hpp
        src/KNN/DistanceCalcs/DistanceCalculator.hpp
        src/KNN/DistanceCalcs/EuclideanDistance.cpp
        src/KNN/DistanceCalcs/EuclideanDistance.hpp
        src/KNN/DistanceCalcs/ManhattanDistance.cpp
        src/KNN/DistanceCalcs/ManhattanDistance.hpp
        src/KNN/Flowers/Flower.cpp
        src/KNN/Flowers/Flower.hpp
        src/KNN/KNNClassifier.cpp
        src/KNN/KNNClassifier.hpp
        src/KNN/KNNFileClassifier.cpp
        src/KNN/KNNFileClassifier.hpp
        src/KNN/Point.cpp
        src/KNN/Point.hpp)
