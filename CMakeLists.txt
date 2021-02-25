cmake_minimum_required(VERSION 3.17)
project(Project)

set(CMAKE_CXX_STANDARD 98)

add_executable(Project main.cpp io/XMLParser.cpp io/XMLParser.h entities/Hub.cpp entities/Hub.h entities/VaccinationCenter.cpp entities/VaccinationCenter.h Simulator.cpp Simulator.h)