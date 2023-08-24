cmake_minimum_required(VERSION 3.11)

project(Cypher CXX)
set(CMAKE_CXX_STANDARD 17)

set(IMGLIB_DIR CACHE STRING "ImgLib static library directory")

if(NOT IMGLIB_DIR)
    message(FATAL_ERROR "Please, specify ImgLib directory via -DIMGLIB_DIR=<dir>")
endif()
message(STATUS "ImgLib dir is ${IMGLIB_DIR}, change via -DIMGLIB_DIR=<dir>")

add_executable(cypher src/main.cpp)

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

target_include_directories(cypher PUBLIC "${IMGLIB_DIR}/include")
target_link_directories(cypher PUBLIC
    "${IMGLIB_DIR}/lib/$<IF:$<CONFIG:Debug>,Debug,Release>"
    )
target_link_libraries(cypher ImgLib ${SYSTEM_LIBS})