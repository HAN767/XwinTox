include(CheckTypeSize)
include(CheckIncludeFiles)

cmake_minimum_required(VERSION 2.8)
project(fastobj C)

CHECK_TYPE_SIZE(int SIZEOF_INT)
CHECK_TYPE_SIZE(long SIZEOF_LONG)
CHECK_TYPE_SIZE("long long" SIZEOF_LONG_LONG)
CHECK_TYPE_SIZE("void *" SIZEOF_VOID_P)
check_include_files(malloc.h HAVE_MALLOC_H)
check_include_files(unistd.h HAVE_UNISTD_H)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/xwfplatf.hin
				${CMAKE_CURRENT_BINARY_DIR}/xwfplatf.h)

file(GLOB_RECURSE SRC_FILES "*.c" "*.cxx")
file(GLOB H_FILES "*.h")
include_directories($(PROJECT_SOURCE_DIR) ${CMAKE_CURRENT_BINARY_DIR})
add_library(fastobj STATIC ${SRC_FILES} ${CMAKE_CURRENT_BINARY_DIR}/lex.cxx)

find_package(FLEX)

find_library(M_LIB m)
set_target_properties(fastobj PROPERTIES PREFIX "")

flex_target(Lexer xwinfast.l  ${CMAKE_CURRENT_BINARY_DIR}/lex.cxx)

install(FILES ${H_FILES} DESTINATION include)
