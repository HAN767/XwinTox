INCLUDE (CheckIncludeFiles)
INCLUDE (CheckLibraryExists)

cmake_minimum_required(VERSION 2.8.0)

CHECK_INCLUDE_FILES (threads.h C11_THREADS)

if(NOT C11_THREADS)
	message("Using internal implementation of C11 threads")
	include_directories("${PROJECT_SOURCE_DIR}/libsunshine/hdr/compat")
endif()

CHECK_LIBRARY_EXISTS(stdthreads "mtx_lock" "" LIBSTDTHREADS)

include_directories("${PROJECT_SOURCE_DIR}/libsunshine/hdr")
include_directories("${PROJECT_SOURCE_DIR}/xwintox/hdr")

add_subdirectory(libsunshine)
add_subdirectory(xwintox)
add_subdirectory(mcom_tox)

add_dependencies(xwintox libsunshine)
