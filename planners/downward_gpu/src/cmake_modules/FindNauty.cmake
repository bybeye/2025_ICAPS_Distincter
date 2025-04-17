# FindNauty.cmake
# Locate the Nauty library and headers

find_path(NAUTY_INCLUDE_DIR
  NAMES nauty.h
  PATHS /usr/local/include /usr/include
)

find_library(NAUTY_LIBRARY
  NAMES nauty
  PATHS /usr/local/lib /usr/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Nauty DEFAULT_MSG
  NAUTY_INCLUDE_DIR
  NAUTY_LIBRARY
)

if(NAUTY_FOUND)
  set(NAUTY_LIBRARIES ${NAUTY_LIBRARY})
  set(NAUTY_INCLUDE_DIRS ${NAUTY_INCLUDE_DIR})
else()
  set(NAUTY_LIBRARIES)
  set(NAUTY_INCLUDE_DIRS)
  message(STATUS "Nauty not found. Please make sure Nauty is installed in /usr/local or /usr and that nauty.h and the necessary libraries are present.")
endif()