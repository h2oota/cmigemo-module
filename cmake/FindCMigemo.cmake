
find_path(CMIGEMO_INCLUDE_DIR migemo.h
  PATHS
    ${MIGEMO_ROOT}
    /usr
    /usr/local
    /opt/local
  PATH_SUFFIXES
    include
)

if(NOT CMIGEMO_INCLUDE_DIR)
    message(FATAL_ERROR "migemo.h not found")
endif()

find_library(CMIGEMO_LIBRARY
  NAMES
    migemo
  PATHS
    ${MIGEMO_ROOT}
    /usr
    /usr/local
    /opt/local
  PATH_SUFFIXES
    lib
)


if(NOT CMIGEMO_INCLUDE_DIR)
    message(FATAL_ERROR "libmigemo not found")
endif()

mark_as_advanced(CMIGEMO_INCLUDE_DIR CMIGEMO_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CMigemo
  REQUIRED_VARS
  CMIGEMO_INCLUDE_DIR
  CMIGEMO_LIBRARY
)

if(CMIGEMO_FOUND AND NOT TARGET CMigemo::CMigemo)
  # GMP::GMPというターゲット名でGMPライブラリを定義
  # UNKNOWN = STATIC/SHAREDかはまだ不明
  # IMPORTED = このプロジェクトに属さないターゲット
  add_library(CMigemo::CMigemo UNKNOWN IMPORTED)
  set_target_properties(CMigemo::CMigemo PROPERTIES
    # C言語、C++なら"CXX"とする
    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    IMPORTED_LOCATION "${CMIGEMO_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${CMIGEMO_INCLUDE_DIR}"
  )
endif()

# Local Variables:
# mode: cmake
# coding: utf-8
# End:
