cmake_minimum_required(VERSION 2.8)
project(forthobj C)

add_executable(mksoftcore softcore/mksoftcore.c)

FUNCTION(PREPEND var prefix)
   SET(listVar "")
   FOREACH(f ${ARGN})
      LIST(APPEND listVar "${prefix}/${f}")
   ENDFOREACH(f)
   SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)

set (SOFTCORE_FILES_P  softcore.fr ifbrack.fr prefix.fr ficl.fr jhlocal.fr
					   marker.fr oo.fr classes.fr string.fr ficllocal.fr 
					   fileaccess.fr xwintox.4th)

PREPEND(SOFTCORE_FILES ${PROJECT_SOURCE_DIR}/softcore/ ${SOFTCORE_FILES_P})

add_custom_command(
OUTPUT ${PROJECT_BINARY_DIR}/softcore.c
COMMAND mksoftcore ${SOFTCORE_FILES}
DEPENDS mksoftcore
MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/softcore/softcore.fr
)

file(GLOB SRC_FILES "*.c" "*.cc")
file(GLOB H_FILES "*.h")
include_directories($(PROJECT_SOURCE_DIR))
add_library(forthobj SHARED ${SRC_FILES} ${PROJECT_BINARY_DIR}/softcore.c)

find_library(M_LIB m)
target_link_libraries(forthobj ${M_LIB})
set_target_properties(forthobj PROPERTIES PREFIX "")

install(TARGETS forthobj LIBRARY DESTINATION lib)
install(FILES ${H_FILES} DESTINATION include)
