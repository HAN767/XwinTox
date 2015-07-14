cmake_minimum_required(VERSION 2.8)
project(xwintox)

file(GLOB_RECURSE SRC_FILES "*.c" "*.cc")
file(GLOB H_FILES "hdr/*.h")
include_directories(${PROJECT_SOURCE_DIR}/hdr $(PROJECT_SOURCE_DIR))
add_executable(xwintox ${SRC_FILES})

if(LIBSTDTHREADS)
	message("Using Libstdthreads")
	target_link_libraries(xwintox stdthreads)
endif()

set_property(TARGET xwintox APPEND PROPERTY COMPILE_DEFINITIONS XWVERS="DR0")
TARGET_LINK_LIBRARIES(xwintox libsunshine)

install(TARGETS xwintox RUNTIME DESTINATION bin)
install(FILES ${H_FILES} DESTINATION include)
