project(dsd)
cmake_minimum_required(VERSION 2.6)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${dsd_SOURCE_DIR}/cmake/")

#include(git_revision)
#git_describe(GIT_TAG)

find_package(LibSndFile REQUIRED)

FILE(GLOB SRCS *.c *.cpp)



INCLUDE_DIRECTORIES(
    "${PROJECT_SOURCE_DIR}"
    "${LIBSNDFILE_INCLUDE_DIR}"
)

SET(LIBS ${LIBS} ${LIBSNDFILE_LIBRARY})

ADD_LIBRARY(dsd ${SRCS})
TARGET_LINK_LIBRARIES(dsd mbe ${GNURADIO_PMT_LIBRARIES} ${LIBS} itpp)
add_definitions(-fPIC)
