cmake_minimum_required(VERSION 3.20)
project(AdvancedProgramming_ass2)

set(CMAKE_CXX_STANDARD 14)

include_directories(serverSrc/KNN)
include_directories(serverSrc/KNN/DistanceCalcs)
include_directories(serverSrc/KNN/Flowers)

add_executable(AdvancedProgramming_ass2
        serverSrc/main.cpp
        serverSrc/KNN/DistanceCalcs/ChebyshevDistance.cpp
        serverSrc/KNN/DistanceCalcs/ChebyshevDistance.hpp
        serverSrc/KNN/DistanceCalcs/DistanceCalculator.hpp
        serverSrc/KNN/DistanceCalcs/EuclideanDistance.cpp
        serverSrc/KNN/DistanceCalcs/EuclideanDistance.hpp
        serverSrc/KNN/DistanceCalcs/ManhattanDistance.cpp
        serverSrc/KNN/DistanceCalcs/ManhattanDistance.hpp
        serverSrc/KNN/Flowers/Flower.cpp
        serverSrc/KNN/Flowers/Flower.hpp
        serverSrc/KNN/KNNClassifier.cpp
        serverSrc/KNN/KNNClassifier.hpp
        serverSrc/KNN/KNNFileClassifier.cpp
        serverSrc/KNN/KNNFileClassifier.hpp
        serverSrc/KNN/Point.cpp
        serverSrc/KNN/Point.hpp)
