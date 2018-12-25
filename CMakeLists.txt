cmake_minimum_required(VERSION 3.5.1)

project(gfs C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(Libgit2 REQUIRED)

set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

if (${DEBUG} AND ${DEBUG} EQUAL 1)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif()

file(GLOB SOURCES src/*.c)
include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES})

if (LIBGIT2_FOUND)
    include_directories(${LIBGIT2_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${LIBGIT2_LIBRARIES})
endif(LIBGIT2_FOUND)


# install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin/)
# file(GLOB HEADERS include/ubiome/fastq/*.h)
# install(FILES ${HEADERS} DESTINATION /usr/include/ubiome/fastq/)

