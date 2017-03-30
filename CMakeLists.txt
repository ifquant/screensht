find_package(PkgConfig REQUIRED)

cmake_minimum_required(VERSION 2.8)
project(screensht C)

pkg_check_modules(X11 x11 REQUIRED)
include_directories(${X11_INCLUDE_DIRS})

pkg_check_modules(JPEG libjpeg REQUIRED)
include_directories(${JPEG_INCLUDE_DIRS})

pkg_check_modules(CURL libcurl REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

pkg_check_modules(GLIB2 glib-2.0 REQUIRED)
include_directories(${GLIB2_INCLUDE_DIRS})

pkg_check_modules(GDK_PIXBUF gdk-pixbuf-2.0 REQUIRED)
include_directories(${GDK_PIXBUF_INCLUDE_DIRS})

pkg_check_modules(LIBNOTIFY libnotify REQUIRED)
include_directories(${LIBNOTIFY_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES src/*.c)
add_executable(screensht ${SOURCES})
target_link_libraries(
        screensht
        ${X11_LIBRARIES}
        ${JPEG_LIBRARIES}
        ${CURL_LIBRARIES}
        ${GLIB2_LIBRARIES}
        ${GDK_PIXBUF_LIBRARIES}
        ${LIBNOTIFY_LIBRARIES}
)
