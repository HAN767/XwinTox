cmake_minimum_required(VERSION 2.8)
project(libsunshine)

file(GLOB_RECURSE SRC_FILES "*.c")
file(GLOB H_FILES "hdr/*.h")
add_library(libsunshine STATIC ${SRC_FILES})
include_directories(${PROJECT_SOURCE_DIR}/hdr)
install(TARGETS libsunshine ARCHIVE DESTINATION lib)
install(FILES ${H_FILES} DESTINATION include)
