cmake_minimum_required(VERSION 2.8)
project(screensht C)

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})
link_directories(${X11_LIBRARIES})

find_package(JPEG REQUIRED)
include_directories(${JPEG_INCLUDE_DIR})
link_directories(${JPEG_LIBRARIES})

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
link_directories(${CURL_LIBRARIES})

file(GLOB SOURCES src/*.c)
add_executable(screensht ${SOURCES})
target_link_libraries(screensht ${X11_LIBRARIES} ${JPEG_LIBRARIES} ${CURL_LIBRARIES})
