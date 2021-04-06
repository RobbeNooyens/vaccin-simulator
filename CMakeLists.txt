cmake_minimum_required(VERSION 3.6)
project(Vaccimulator)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(gtest/include)

# Set Library dir
link_directories(gtest/lib)

# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES Vaccimulator.cpp
        tinyxml/tinyxmlparser.cpp
        tinyxml/tinyxmlerror.cpp
        tinyxml/tinyxml.h tinyxml/tinyxml.cpp
        tinyxml/tinystr.h tinyxml/tinystr.cpp
        io/XMLParser.cpp io/XMLParser.h
        entities/Hub.cpp entities/Hub.h
        entities/VaccinationCenter.cpp entities/VaccinationCenter.h
        Simulator.cpp Simulator.h
        json/JValue.cpp json/JValue.h
        json/JObject.cpp json/JObject.h
        json/JArray.cpp json/JArray.h
        DesignByContract.h
        utils.cpp utils.h)

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES VaccimulatorTests.cpp
        utils.h utils.cpp
        tests/domain/VaccinationCenterTests.cpp
        tests/domain/HubTests.cpp
        tests/presentation/HubOutTests.cpp
        tests/data/XMLParserTests.cpp
        tests/data/JsonTests.cpp
        tinyxml/tinyxmlparser.cpp
        tinyxml/tinyxmlerror.cpp
        tinyxml/tinyxml.h tinyxml/tinyxml.cpp
        tinyxml/tinystr.h tinyxml/tinystr.cpp
        io/XMLParser.cpp io/XMLParser.h
        entities/Hub.cpp entities/Hub.h
        entities/VaccinationCenter.cpp entities/VaccinationCenter.h
        Simulator.cpp Simulator.h tests/domain/HubTests.cpp
        json/JValue.cpp json/JValue.h
        json/JObject.cpp json/JObject.h
        json/JArray.cpp json/JArray.h
        DesignByContract.h)

# Create RELEASE target
add_executable(Vaccimulator ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(VaccimulatorTests ${DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(VaccimulatorTests gtest)