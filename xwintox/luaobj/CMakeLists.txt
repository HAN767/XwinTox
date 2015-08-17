cmake_minimum_required(VERSION 2.8)
project(luaobj C)

file(GLOB SRC_FILES "*.c" "*.cxx")
file(GLOB H_FILES "*.h")
include_directories($(PROJECT_SOURCE_DIR))
add_library(luaobj STATIC ${SRC_FILES})

find_library(M_LIB m)
set_target_properties(luaobj PROPERTIES PREFIX "")

install(FILES ${H_FILES} DESTINATION include)
