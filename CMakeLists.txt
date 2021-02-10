cmake_minimum_required(VERSION 3.10)
set(PROPERTYSDK_NAME propertysdk)
project(${PROPERTYSDK_NAME})

set(PROPERTYSDK_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(PROPERTYSDK_BINARY_DIR ${PROJECT_BINARY_DIR})
set(PROPERTYSDK_OUTPUT_DIR
    "${PROPERTYSDK_BINARY_DIR}/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

option(BUILD_SHARED_LIBS "The shared library" ON)
option(PROPERTYSDK_EXAMPLE "Enable examples" ON)
option(PROPERTYSDK_TEST "Build feature_traker test" ON)

option(PROPERTYSDK_AUTO_CLANG_TIDY "Enable autotidy with clang-tidy" ON)
option(PROPERTYSDK_AUTO_CLANG_FORMAT "Enable autoformat with clang-format" ON)

if(MSVC)
  if(BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
  endif()
  if(UNIX)
    message(STATUS "Use position independent code")
    set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ON)
  endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Build configuration 'Release' or 'Debug'." FORCE)
endif()

# cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(Utils)
include(clang-tidy)
include(depends)

add_subdirectory(src)

# example
if(PROPERTYSDK_EXAMPLE)
  add_subdirectory(example)
endif()

log(TEXT
    "Target name                                    ${PROPERTYSDK_NAME}"
    "Target config                              --> ${CMAKE_BUILD_TYPE}"
    "Build shared library                       --> ${BUILD_SHARED_LIBS}"
    "Build feature_traker examples                  ${PROPERTYSDK_EXAMPLE}"
    "Build feature_traker test                      ${PROPERTYSDK_TEST}"
    "Auto clang-tidy                                ${PROPERTYSDK_AUTO_CLANG_TIDY}"
    "Auto clang-format                              ${PROPERTYSDK_AUTO_CLANG_FORMAT}"
    SUMMARIZE)
