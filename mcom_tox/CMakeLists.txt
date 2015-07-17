include (FindPkgConfig)

cmake_minimum_required(VERSION 2.8)
project(mcommtox)

pkg_check_modules(libtoxcore REQUIRED libtoxcore)

link_directories(${libtoxcore_LIBRARY_DIRS})
include_directories(${libtoxcore_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/hdr)

file(GLOB_RECURSE SRC_FILES "*.cc")
file(GLOB H_FILES "hdr/*.h")

add_library(mcommtox SHARED ${SRC_FILES})
target_link_libraries(mcommtox ${libtoxcore_LIBRARIES})
install(TARGETS mcommtox LIBRARY DESTINATION lib)
install(FILES ${H_FILES} DESTINATION include)
