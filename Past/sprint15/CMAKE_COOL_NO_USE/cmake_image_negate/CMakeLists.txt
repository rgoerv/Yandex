cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(PROJECT_NAME Negate)

project(${PROJECT_NAME} CXX)
set(TARGET_NAME negate)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE DEBUG)

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

add_executable(${TARGET_NAME} src/main.cpp src/img_lib.h src/img_lib.cpp src/ppm_image.h src/ppm_image.cpp)
target_link_libraries(${TARGET_NAME} ${SYSTEM_LIBS})