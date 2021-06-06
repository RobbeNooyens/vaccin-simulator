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
set(RELEASE_SOURCE_FILES src/Vaccimulator.cpp
        tinyxml/tinyxmlparser.cpp
        tinyxml/tinyxmlerror.cpp
        tinyxml/tinyxml.h tinyxml/tinyxml.cpp
        tinyxml/tinystr.h tinyxml/tinystr.cpp
        src/parsing/XMLParser.cpp src/parsing/XMLParser.h
        src/entities/Hub.cpp src/entities/Hub.h
        src/entities/VaccinationCenter.cpp src/entities/VaccinationCenter.h
        src/entities/Simulator.cpp src/entities/Simulator.h
        src/entities/Vaccine.cpp src/entities/Vaccine.h
        src/json/JValue.cpp src/json/JValue.h
        src/json/JObject.cpp src/json/JObject.h
        src/json/JArray.cpp src/json/JArray.h
        src/json/JKeys.h
        DesignByContract.h
        src/utilities/utils.cpp src/utilities/utils.h
        tests/TestUtils.h tests/TestUtils.cpp
        src/entities/Planning.cpp src/entities/Planning.h
        src/entities/SimulationData.cpp src/entities/SimulationData.h)

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES tests/VaccimulatorTests.cpp
        src/utilities/utils.h src/utilities/utils.cpp
        tests/domain/VaccinationCenterTests.cpp
        tests/domain/HubTests.cpp
        tests/domain/VaccineTests.cpp
        tests/presentation/SimulatorOutputTests.cpp
        tests/data/XMLParserTests.cpp
        tests/data/JsonTests.cpp
        tests/TestUtils.cpp tests/TestUtils.h
        tinyxml/tinyxmlparser.cpp
        tinyxml/tinyxmlerror.cpp
        tinyxml/tinyxml.h tinyxml/tinyxml.cpp
        tinyxml/tinystr.h tinyxml/tinystr.cpp
        src/parsing/XMLParser.cpp src/parsing/XMLParser.h
        src/entities/Hub.cpp src/entities/Hub.h
        src/entities/VaccinationCenter.cpp src/entities/VaccinationCenter.h
        src/entities/Simulator.cpp src/entities/Simulator.h
        src/entities/Vaccine.cpp src/entities/Vaccine.h
        src/entities/Planning.cpp src/entities/Planning.h
        src/entities/SimulationData.cpp src/entities/SimulationData.h
        src/json/JValue.cpp src/json/JValue.h
        src/json/JObject.cpp src/json/JObject.h
        src/json/JArray.cpp src/json/JArray.h
        src/json/JKeys.h
        DesignByContract.h
        tests/domain/PlanningTests.cpp)

# Set source files for single DEBUG target
set(SINGLE_DEBUG_SOURCE_FILES tests/SingleTests.cpp
        src/utilities/utils.h src/utilities/utils.cpp
        tests/TestUtils.cpp tests/TestUtils.h
        tinyxml/tinyxmlparser.cpp
        tinyxml/tinyxmlerror.cpp
        tinyxml/tinyxml.h tinyxml/tinyxml.cpp
        tinyxml/tinystr.h tinyxml/tinystr.cpp
        src/parsing/XMLParser.cpp src/parsing/XMLParser.h
        src/entities/Hub.cpp src/entities/Hub.h
        src/entities/VaccinationCenter.cpp src/entities/VaccinationCenter.h
        src/entities/Simulator.cpp src/entities/Simulator.h
        src/entities/Vaccine.cpp src/entities/Vaccine.h
        src/entities/Planning.cpp src/entities/Planning.h
        src/entities/SimulationData.cpp src/entities/SimulationData.h
        src/json/JValue.cpp src/json/JValue.h
        src/json/JObject.cpp src/json/JObject.h
        src/json/JArray.cpp src/json/JArray.h
        src/json/JKeys.h
        DesignByContract.h)


# Create RELEASE target
add_executable(Vaccimulator ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(VaccimulatorTests ${DEBUG_SOURCE_FILES})
add_executable(SingleTests ${SINGLE_DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(VaccimulatorTests gtest)