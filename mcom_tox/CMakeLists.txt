include (FindPkgConfig)

cmake_minimum_required(VERSION 2.8)
project(mcom_tox)

pkg_check_modules(libtoxcore REQUIRED libtoxcore)

link_directories(${libtoxcore_LIBRARY_DIRS})
include_directories(${libtoxcore_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/hdr)

file(GLOB_RECURSE SRC_FILES "*.cc")
file(GLOB H_FILES "hdr/*.h")

add_library(mcom_tox SHARED ${SRC_FILES})
target_link_libraries(mcom_tox ${libtoxcore_LIBRARIES} libsunshine)
install(TARGETS mcom_tox LIBRARY DESTINATION lib)
install(FILES ${H_FILES} DESTINATION include)
set_target_properties(mcom_tox PROPERTIES PREFIX "")
