cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

set(PROJECT_NAME Negate)

project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE DEBUG)

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

set(SOURCE_DIR src)
set(IMGLIB_FILES "${SOURCE_DIR}/img_lib.h" "${SOURCE_DIR}/img_lib.cpp" "${SOURCE_DIR}/ppm_image.h" "${SOURCE_DIR}/ppm_image.cpp")

set(FILE_MAIN "${SOURCE_DIR}/vmirr.cpp")
set(TARGET_NAME vmirror)

add_executable(${TARGET_NAME} ${FILE_MAIN} ${IMGLIB_FILES})
target_link_libraries(${TARGET_NAME} ${SYSTEM_LIBS})

set(FILE_MAIN "${SOURCE_DIR}/hmirr.cpp")
set(TARGET_NAME hmirror)

add_executable(${TARGET_NAME} ${FILE_MAIN} ${IMGLIB_FILES})
target_link_libraries(${TARGET_NAME} ${SYSTEM_LIBS})

set(FILE_MAIN "${SOURCE_DIR}/sobel.cpp")
set(TARGET_NAME sobel)

add_executable(${TARGET_NAME} ${FILE_MAIN} ${IMGLIB_FILES})
target_link_libraries(${TARGET_NAME} ${SYSTEM_LIBS})