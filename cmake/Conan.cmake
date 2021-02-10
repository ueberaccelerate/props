macro(run_conan)
  # Download automatically, you can also just copy the conan.cmake file
  if(NOT EXISTS "${PROJECT_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
         "${PROJECT_BINARY_DIR}/conan.cmake")
  endif()

  include(${PROJECT_BINARY_DIR}/conan.cmake)

  conan_add_remote(NAME bincrafters URL https://api.bintray.com/conan/bincrafters/public-conan)
  if(WIN32)
    if(NOT CONAN_BINARY_IMPORT_DIR)
      message(WARNING "Conan import folder: ${PROJECT_BINARY_DIR}/bin ")
      set(CONAN_BINARY_IMPORT_DIR ${PROJECT_BINARY_DIR}/bin)
    endif()
  endif()

  # see https://conan.io/center/ for package
  conan_cmake_run(
    REQUIRES
    gtest/1.10.0
    BASIC_SETUP
    CMAKE_TARGETS # individual targets to link to
    BUILD
    missing
    OPTIONS
    gtest:shared=True
    IMPORTS
    "bin, *.dll -> ${CONAN_BINARY_IMPORT_DIR}")

  include(${PROJECT_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup()
endmacro()
