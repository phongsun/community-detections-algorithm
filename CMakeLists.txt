cmake_minimum_required(VERSION 3.14)
project(21s-pa03-peter-sun)
set (EXE_NAME social-graph)
#set (TEST_NAME tests)



##########################################
# Students, edit the items below.  In the quotes,
# put the names of the input files as they should appear
# in the argv param to main.
# Example:
#   set (input01 "train_data.csv")
#   set (input02 "train_target.csv")
# etc....

set (input01 "data/put_data_here.txt")
#set (input02 "")

#
##########################################

# Copy data files to build dir
foreach(file IN LISTS input01)
    configure_file(${file} ${file} COPYONLY)
endforeach()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message(" ")
    message("CMAKE IN DEBUG MODE")
    message(" ")

    set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    message(" ")
    message("CMAKE IN RELEASE MODE")
    message(" ")

    SET(GCC_COVERAGE_COMPILE_FLAGS "-O3")
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
endif ()

include_directories(src)
include_directories(test)

set(CMAKE_CXX_STANDARD 17)


set(COMMON_SOURCES)
set(TARGET_SOURCES src/main.cpp src/GVGraph.cpp src/GVGraph.h)
#set(TEST_SOURCES test/catch.hpp test/test.cpp test/TestSortingAlgos.cpp)

add_executable(${EXE_NAME} ${COMMON_SOURCES} ${TARGET_SOURCES})
#add_executable(${TEST_NAME} ${COMMON_SOURCES} ${TEST_SOURCES})

Set(BOOST_ROOT /usr/local/boost_1_75_0)
find_package(Boost)
if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${BOOST_ROOT})
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
    target_link_libraries(${EXE_NAME} ${Boost_LIBRARIES})
endif()