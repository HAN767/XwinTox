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
MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/softcore/xwintox.4th
)

include_directories($(PROJECT_SOURCE_DIR))
add_library(forthobj STATIC FORTHOBJ.cc bit.c callback.c unix.c utility.c vm.c
			dictionary.c double.c extras.c fileaccess.c float.c hash.c
			main.c modFOBJ.cc prefix.c primitives.c search.c stack.c system.c
			tools.c word.c ${PROJECT_BINARY_DIR}/softcore.c)

find_library(M_LIB m)
target_link_libraries(forthobj ${M_LIB})
set_target_properties(forthobj PROPERTIES PREFIX "")
