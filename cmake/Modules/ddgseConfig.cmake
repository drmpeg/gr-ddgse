if(NOT PKG_CONFIG_FOUND)
    INCLUDE(FindPkgConfig)
endif()
PKG_CHECK_MODULES(PC_DDGSE ddgse)

FIND_PATH(
    DDGSE_INCLUDE_DIRS
    NAMES ddgse/api.h
    HINTS $ENV{DDGSE_DIR}/include
        ${PC_DDGSE_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    DDGSE_LIBRARIES
    NAMES gnuradio-ddgse
    HINTS $ENV{DDGSE_DIR}/lib
        ${PC_DDGSE_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/ddgseTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DDGSE DEFAULT_MSG DDGSE_LIBRARIES DDGSE_INCLUDE_DIRS)
MARK_AS_ADVANCED(DDGSE_LIBRARIES DDGSE_INCLUDE_DIRS)
