
cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(migemo-cmigemo
    VERSION 1.0
    DESCRIPTION "C/Migemo emacs module"
    LANGUAGES C
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# prevent in-source build
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX /usr/local/share/emacs/site-lisp CACHE PATH "..." FORCE)
endif()

find_package(Emacs REQUIRED)
find_package(CMigemo REQUIRED)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/migemo
  COMMAND mkdir ${CMAKE_CURRENT_BINARY_DIR}/migemo
)

add_library(migemo-cmigemo
  MODULE src/emacs-module.c
)

set_target_properties(migemo-cmigemo
  PROPERTIES
  PREFIX ""
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/migemo"
)
target_link_libraries(migemo-cmigemo Emacs::Emacs CMigemo::CMigemo)

get_property(EMACS_COMMAND TARGET Emacs::Emacs PROPERTY COMMAND)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/migemo/migemo.el ${CMAKE_CURRENT_BINARY_DIR}/migemo/migemo.elc
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/migemo/migemo.el ${CMAKE_CURRENT_BINARY_DIR}/migemo
  COMMAND ${EMACS_COMMAND} -batch -f batch-byte-compile ${CMAKE_CURRENT_BINARY_DIR}/migemo/migemo.el
  DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/migemo/migemo.el
)

if(INSTALL_FLAT)
  set(INSTALL_DIR ".")
else()
  set(INSTALL_DIR "migemo")
endif()

add_custom_target(ELISP ALL
  DEPENDS
  ${CMAKE_CURRENT_BINARY_DIR}/migemo
  ${CMAKE_CURRENT_BINARY_DIR}/migemo/migemo.el
  ${CMAKE_CURRENT_BINARY_DIR}/migemo/migemo.elc
)

install(
  TARGETS migemo-cmigemo
  DESTINATION ${INSTALL_DIR}
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/migemo/migemo.el ${CMAKE_CURRENT_BINARY_DIR}/migemo/migemo.elc
  DESTINATION  ${INSTALL_DIR}
  COMPONENT Runtime
)

# Local Variables:
# mode: cmake
# coding: utf-8
# End:
