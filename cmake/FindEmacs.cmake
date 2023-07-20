find_path(EMACS_INCLUDE_DIR emacs-module.h
  PATHS
    ENV EMACS_ROOT
    ENV EMACS_INCLUDE_DIR
    ${EMACS_ROOT}
    /Applications/Emacs.app/Contents/Resources
    /Applications/MacPorts/Emacs.app/Contents/Resources
    /Applications/MacPorts/EmacsMac.app/Contents/Resources
  PATH_SUFFIXES
    include
  )


if(NOT EMACS_INCLUDE_DIR)
    message(FATAL_ERROR "emacs-module.h not found")
endif()

if (APPLE)
  find_program(EMACS_EXECUTABLE
    Emacs
    PATHS
    /Applications/MacPorts/EmacsMac.app/Contents/MacOS
    /Applications/MacPorts/Emacs.app/Contents/MacOS
    /Applications/Emacs.app/Contents/MacOS
    ENV PATH
    NO_DEFAULT_PATH
  )

  if(NOT EMACS_EXECUTABLE)
    message(FATAL_ERROR "Emacs not found.")
  endif()
else()
  set(EMACS_EXECUTABLE "emacs")
endif()

mark_as_advanced(EMACS_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Emacs
  REQUIRED_VARS EMACS_INCLUDE_DIR EMACS_EXECUTABLE
)

if(Emacs_FOUND AND NOT TARGET Emacs::Emacs)
  add_library(Emacs::Emacs INTERFACE IMPORTED)
  define_property(TARGET PROPERTY "COMMAND")
  set_target_properties(Emacs::Emacs PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${EMACS_INCLUDE_DIR}"
    COMMAND "${EMACS_EXECUTABLE}"
  )
endif()

# Local Variables:
# mode: cmake
# coding: utf-8
# End:
