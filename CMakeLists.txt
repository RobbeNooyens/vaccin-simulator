cmake_minimum_required(VERSION 3.17)
project(Project)

set(CMAKE_CXX_STANDARD 98)

add_executable(Project main.cpp io/XMLParser.cpp io/XMLParser.h domain/Hub.cpp domain/Hub.h domain/VaccinatieCentrum.cpp domain/VaccinatieCentrum.h Simulator.cpp Simulator.h)