#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "TorchSparse::TorchSparse" for configuration ""
set_property(TARGET TorchSparse::TorchSparse APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(TorchSparse::TorchSparse PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_NOCONFIG "torch;Python3::Python"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libtorchsparse.so"
  IMPORTED_SONAME_NOCONFIG "libtorchsparse.so"
  )

list(APPEND _cmake_import_check_targets TorchSparse::TorchSparse )
list(APPEND _cmake_import_check_files_for_TorchSparse::TorchSparse "${_IMPORT_PREFIX}/lib/libtorchsparse.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
