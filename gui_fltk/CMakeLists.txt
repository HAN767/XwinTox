cmake_minimum_required(VERSION 2.8)
project(gui_fltk)

#add_library(FLTK STATIC IMPORTED)
find_package(FLTK REQUIRED)

include_directories(${FLTK_INCLUDE_DIR} ${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE SRC_FILES "*.c")
file(GLOB_RECURSE SRC_FILES "*.cc")
file(GLOB H_FILES "hdr/*.h")

SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
add_library(gui_fltk SHARED ${SRC_FILES})
target_link_libraries(gui_fltk ${FLTK_LIBRARIES})
install(TARGETS gui_fltk LIBRARY DESTINATION lib)
set_target_properties(gui_fltk PROPERTIES PREFIX "")
