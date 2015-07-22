include (FindPkgConfig)

cmake_minimum_required(VERSION 2.8)
project(toxdns3)

pkg_check_modules(libtoxcore REQUIRED libtoxcore)

link_directories(${libtoxcore_LIBRARY_DIRS})
include_directories(${libtoxcore_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/hdr)

file(GLOB_RECURSE SRC_FILES "*.cc")

add_library(toxdns3 SHARED ${SRC_FILES})
target_link_libraries(toxdns3 ${libtoxcore_LIBRARIES} libsunshine)
install(TARGETS toxdns3 LIBRARY DESTINATION lib)
install(FILES ${H_FILES} DESTINATION include)
set_target_properties(toxdns3 PROPERTIES PREFIX "")
