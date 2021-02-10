# include CPM Package Management
include(get_cpm)

# find_package https://cmake.org/cmake/help/v3.15/command/find_package.html?highlight=find_package
# ==============================================================================
# * add Find<PackageName>.cmake to cmake/modules folder
# * see https://github.com/rpavlik/cmake-modules -
# ==============================================================================

# include Conan Package Management
# ==============================================================================
# * install conan executable with 'python -m pip install conan'
# * setup CONAN_BINARY_IMPORT_DIR to import binaries to destination folder
# * set(CONAN_BINARY_IMPORT_DIR /path_to_output_folder)
# * In Code:
# * run_conan()
# ==============================================================================
# include(Conan)

# CPM
# ==============================================================================
# * add_<PackageName>_package ( TARGET TARGET_ACCESS )
# * Example
# * function (add_easy_prolifer_package target_package target_access_package)
# * ...
# * endfunction()
# * In Code:
# * add_easy_prolifer_package ( superfeature PRIVATE)
# ==============================================================================

# Easy Profiler Package
function(add_easy_profiler_package target target_access)
  # see https://github.com/TheLartians/CPM.cmake#usage CPMFindPackage will try to find a local
  # dependency via CMake's find_package and fallback to CPMAddPackage
  cpmfindpackage(
    NAME
    easy_profiler
    GITHUB_REPOSITORY
    yse/easy_profiler
    # Version 2.1.0
    GIT_TAG
    046d9bd
    OPTIONS
    "EASY_PROFILER_NO_GUI ON"
    "EASY_PROFILER_NO_SAMPLES ON")

  target_link_libraries(${target} ${target_access} easy_profiler)
  set_target_properties(
    easy_profiler
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
               LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
               RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

endfunction()

# Google Test
function(add_google_test target target_access)
  cpmfindpackage(
    NAME
    googletest
    GITHUB_REPOSITORY
    google/googletest
    # Version v1.10.0
    GIT_TAG
    703bd9c
    OPTIONS
    "BUILD_GMOCK OFF"
    "gtest_force_shared_crt on")
  set_target_properties(
    gtest gtest_main
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
               LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
               RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

  target_link_libraries(${target} ${target_access} gtest gtest_main)
endfunction()
