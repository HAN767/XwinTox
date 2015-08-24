cmake_minimum_required(VERSION 2.8)
project(xwintox)

file(GLOB H_FILES "hdr/*.h")
include_directories(${PROJECT_SOURCE_DIR}/hdr $(PROJECT_SOURCE_DIR))
add_executable(xwintox xwintox.c module/manager.c)

if(LIBSTDTHREADS)
	message("Using Libstdthreads")
	target_link_libraries(xwintox stdthreads)
endif()

set_property(TARGET xwintox APPEND PROPERTY COMPILE_DEFINITIONS XWVERS="DR0")
TARGET_LINK_LIBRARIES(xwintox ${CMAKE_DL_LIBS} libsunshine luaobj fastobj)

add_subdirectory(fastobj)
add_subdirectory(luaobj)

install(TARGETS xwintox RUNTIME DESTINATION bin)
install(FILES ${H_FILES} DESTINATION include)
