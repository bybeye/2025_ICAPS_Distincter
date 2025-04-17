#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "TorchScatter::TorchScatter" for configuration ""
set_property(TARGET TorchScatter::TorchScatter APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(TorchScatter::TorchScatter PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_NOCONFIG "torch;Python3::Python"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libtorchscatter.so"
  IMPORTED_SONAME_NOCONFIG "libtorchscatter.so"
  )

list(APPEND _cmake_import_check_targets TorchScatter::TorchScatter )
list(APPEND _cmake_import_check_files_for_TorchScatter::TorchScatter "${_IMPORT_PREFIX}/lib/libtorchscatter.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
