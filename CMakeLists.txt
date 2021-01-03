cmake_minimum_required(VERSION 3.15)
project(Botvina2HGeant)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
list(APPEND CMAKE_PREFIX_PATH $ENV{McIniData_DIR})
#list(APPEND CMAKE_PREFIX_PATH ${McIniData_DIR})

find_package(ROOT REQUIRED COMPONENTS RIO Tree)
#find_package(McIniData REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall")
include(${ROOT_USE_FILE})

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${ROOT_INCLUDE_DIRS}
        $ENV{McIniData_DIR}/include
)

add_executable(convert src/convert.cc)
target_link_libraries(convert $ENV{McIniData_DIR}/build/libMcIniData.so ${ROOT_LIBRARIES} ROOT::EG)