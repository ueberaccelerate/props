find_program(CLANG_TIDY clang-tidy)
if(NOT CLANG_TIDY)
  message(STATUS "Did not find clang-tidy, target tidy is disabled.")

  macro(clang_tidy directory DESC)

  endmacro(clang_tidy)
  macro(clang_tidy_recurse directory DESC)

  endmacro(clang_tidy_recurse)
else()
  message(STATUS "Found clang-tidy, use \"make tidy\" to run it.")
  set(CLANG_TIDY_FOUND
      true
      CACHE BOOL "Found clang-tidy.")

  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") # Matches "Clang" and "AppleClang"
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5)
      message(STATUS "Please enable readability-redundant-member-init (disabled due to #32966)")
    endif()
  endif()

  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  option (CLANG_TIDY_AUTOFIX "Auto fix errors" ON)
  function(clang_tidy)
    set(oneValueArgs TARGET DIRECTORY)
    cmake_parse_arguments(CT "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    # keep all sources
    file(GLOB_RECURSE CT_${CT_TARGET}_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${CT_DIRECTORY}/*.cpp")

    # get target include directories
    get_target_property(LINK_TARGETS ${CT_TARGET} LINK_LIBRARIES)
    get_target_property(CT_TARGET_INCLUDE_DIRECTORIES ${CT_TARGET} INCLUDE_DIRECTORIES)
    
    list(LENGTH CT_TARGET_INCLUDE_DIRECTORIES CT_TARGET_INCLUDE_DIRECTORIES_LENGTH)
    # 
    if( NOT CT_TARGET_INCLUDE_DIRECTORIES_LENGTH EQUAL 1)
      list(POP_BACK CT_TARGET_INCLUDE_DIRECTORIES)
    endif()

    foreach(target ${LINK_TARGETS})
      get_target_property(interface_dirs ${target} INTERFACE_INCLUDE_DIRECTORIES)
      get_target_property(dirs ${target} INCLUDE_DIRECTORIES)

      if(interface_dirs)
        list(APPEND CT_TARGET_INCLUDE_DIRECTORIES ${interface_dirs})
      endif()
      if(dirs)
        list(APPEND CT_TARGET_INCLUDE_DIRECTORIES ${dirs})
      endif()
    endforeach(target)
    
    foreach(src ${CT_TARGET_INCLUDE_DIRECTORIES})
      list(APPEND CLANG_TIDY_PUBLIC_HEADER_PREFIX -I ${src})
    endforeach(src)

    set(CLANG_TIDY_COMPILE_FLAGS -- ${CLANG_TIDY_PUBLIC_HEADER_PREFIX} -std=c++17)

    if(EXISTS ${PROJECT_BINARY_DIR}/compile_commands.json)
      set(CLANG_TIDY_COMPILE_COMMANDS -p ${PROJECT_SOURCE_DIR}/compile_commands.json)
    endif()
    
    list(APPEND CLANG_TIDY_FLAGS ${CLANG_TIDY_COMPILE_COMMANDS})
    if(CLANG_TIDY_AUTOFIX)
      list(APPEND CLANG_TIDY_FLAGS -fix)
    endif()
    
    add_custom_target(
      tidy_${CT_TARGET}
      COMMAND ${CLANG_TIDY} ${CT_${CT_TARGET}_FILES} ${CLANG_TIDY_FLAGS}
              ${CLANG_TIDY_COMPILE_FLAGS}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      COMMAND_EXPAND_LISTS)
  endfunction()
endif()

find_program(CLANG_FORMAT clang-format)
if(NOT CLANG_FORMAT)
  message(STATUS "Did not find clang-format, target format is disabled.")
else()
  message(STATUS "Found clang-format, use \"make format\" to run it.")
  if(WIN32)
    log(TEXT "Clang Format native support. (Visual Studio)")
    add_custom_target(format)
  else()
    add_custom_target(
      format
      COMMAND find ./ -iname "*.h" -o -iname "*.hpp" -o -iname "*.cpp" | xargs ${CLANG_FORMAT}
              -style=file -i -verbose
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
  endif()
endif()

mark_as_advanced(CLANG_TIDY_FOUND)
