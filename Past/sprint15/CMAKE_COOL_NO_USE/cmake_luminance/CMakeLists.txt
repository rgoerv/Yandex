cmake_minimum_required(VERSION 3.11)

project(LumaCorr CXX)
set(CMAKE_CXX_STANDARD 17)

set(IMGLIB_DLL_DIR CACHE STRING "ImgLibDll shared library directory")

if(NOT IMGLIB_DLL_DIR)
    message(FATAL_ERROR "Please, specify ImgLibDll directory via -DIMGLIB_DLL_DIR=<dir>")
endif()
message(STATUS "ImgLibDll dir is ${IMGLIB_DLL_DIR}, change via -DIMGLIB_DLL_DIR=<dir>")

if (CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

add_executable(lumacorr main.cpp)

target_include_directories(lumacorr PUBLIC "${IMGLIB_DLL_DIR}/include")

# В команде target_link_libraries можно указывать полный путь к файлу библиотеки
# Для dll-файла укажите файл libImgLibDll.dll.a
target_link_libraries(lumacorr "${IMGLIB_DLL_DIR}/lib/libImgLibDll.so" ${SYSTEM_LIBS})