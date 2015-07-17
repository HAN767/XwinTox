cmake_minimum_required(VERSION 2.8)
project(libsunshine)

file(GLOB_RECURSE SRC_FILES "*.c")
file(GLOB H_FILES "hdr/*.h")

add_library(libsunshine STATIC ${SRC_FILES})
if(NOT C11_THREADS)
	message("Linking libsunshine with pthread")
	target_link_libraries(libsunshine pthread)
endif()

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-fPIC")

include_directories(${PROJECT_SOURCE_DIR}/hdr)

install(TARGETS libsunshine ARCHIVE DESTINATION lib)
install(FILES ${H_FILES} DESTINATION include)
