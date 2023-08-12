cmake_minimum_required(VERSION 3.11)

project(ImgLib CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE DEBUG)

set(SOURCE_DIR src)
set(IMGLIB_MAIN_FILES "${SOURCE_DIR}/img_lib.h" "${SOURCE_DIR}/img_lib.cpp")
set(IMGLIB_FORMAT_FILES "${SOURCE_DIR}/ppm_image.h" "${SOURCE_DIR}/ppm_image.cpp")

add_library(ImgLib STATIC ${IMGLIB_MAIN_FILES} ${IMGLIB_FORMAT_FILES})