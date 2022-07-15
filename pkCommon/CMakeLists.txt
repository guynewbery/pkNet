add_library (pkcommon hello.cxx)

# Make sure the compiler can find include files for our pkcommon library
# when other libraries or executables link to pkcommon
target_include_directories (pkcommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})