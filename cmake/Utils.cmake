function(log)
  set(options SUMMARIZE)
  set(multiValueArgs TEXT)
  cmake_parse_arguments(FTLOG "${options}" "" "${multiValueArgs}" ${ARGN})
  if(FTLOG_SUMMARIZE)
    message(STATUS "################################################################")
  endif()

  foreach(src ${FTLOG_TEXT})
    message(STATUS "  ${src}.")
  endforeach(src)

  if(FTLOG_SUMMARIZE)
    message(STATUS "################################################################")
  endif()
endfunction()

function(add_target)
  set(oneValueArgs TARGET PREFIX PUBLIC_PREFIX BINARY_DIRECTORY)
  set(multiValueArgs
      PUBLIC_HEADER
      PRIVATE_HEADER
      PUBLIC_SOURCE
      PRIVATE_SOURCE
      SOURCE
      PUBLIC_LINKS
      PRIVATE_LINKS)
  cmake_parse_arguments(FT "EXECUTABLE" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(NOT FT_PREFIX)
    set(FT_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
  endif()
  log(TEXT
      "Target          - ${FT_TARGET}"
      "Prefix          - ${FT_PREFIX}"
      "Public Prefix   - ${FT_PUBLIC_PREFIX}"
      "* Public headers"
      "${FT_PUBLIC_HEADER}"
      "* Private headers"
      "${FT_PRIVATE_HEADER}"
      "* Public sources"
      "${FT_PUBLIC_SOURCE}"
      "* Private sources"
      "${FT_PRIVATE_SOURCE}"
      "${FT_SOURCE}"
      SUMMARIZE)

  foreach(src ${FT_PUBLIC_HEADER})
    list(APPEND FT_PUBLIC_HEADER_WITH_PREFIX ${FT_PUBLIC_PREFIX}/${src})
  endforeach(src)
  foreach(src ${FT_PUBLIC_SOURCE})
    list(APPEND FT_PUBLIC_SOURCE_WITH_PREFIX ${FT_PUBLIC_PREFIX}/${src})
  endforeach(src)

  foreach(src ${FT_PRIVATE_HEADER})
    list(APPEND FT_PRIVATE_HEADER_WITH_PREFIX ${FT_PREFIX}/${src})
  endforeach(src)

  foreach(src ${FT_PRIVATE_SOURCE})
    list(APPEND FT_PRIVATE_SOURCE_WITH_PREFIX ${FT_PREFIX}/${src})
  endforeach(src)

  foreach(src ${FT_SOURCE})
    list(APPEND FT_SOURCE_WITH_PREFIX ${FT_PREFIX}/${src})
  endforeach(src)

  if(NOT FT_EXECUTABLE)
    add_library(
      ${FT_TARGET}
      ${FT_PRIVATE_HEADER_WITH_PREFIX} ${FT_PRIVATE_SOURCE_WITH_PREFIX}
      ${FT_PUBLIC_HEADER_WITH_PREFIX} ${FT_PUBLIC_SOURCE_WITH_PREFIX} ${FT_SOURCE_WITH_PREFIX})
  else()
    add_executable(
      ${FT_TARGET}
      ${FT_PRIVATE_HEADER_WITH_PREFIX} ${FT_PRIVATE_SOURCE_WITH_PREFIX}
      ${FT_PUBLIC_HEADER_WITH_PREFIX} ${FT_PUBLIC_SOURCE_WITH_PREFIX} ${FT_SOURCE_WITH_PREFIX})
  endif()

  if(FT_BINARY_DIRECTORY)
    set_target_properties(${FT_TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${FT_BINARY_DIRECTORY}"
                                                  RUNTIME_OUTPUT_DIRECTORY "${FT_BINARY_DIRECTORY}")
  endif()

  target_include_directories(${FT_TARGET} PUBLIC ${FT_PUBLIC_PREFIX})

  target_link_libraries(
    ${FT_TARGET}
    PUBLIC ${FT_PUBLIC_LINKS}
    PRIVATE ${FT_PRIVATE_LINKS})

  if(FT_PUBLIC_PREFIX)
    source_group(
      TREE ${FT_PUBLIC_PREFIX}
      PREFIX "Header Files/Public"
      FILES ${FT_PUBLIC_HEADER_WITH_PREFIX})
    source_group(
      TREE ${FT_PUBLIC_PREFIX}
      PREFIX "Source Files/Public"
      FILES ${FT_PUBLIC_SOURCE_WITH_PREFIX})
  endif()
  if(FT_PREFIX)
    source_group(
      TREE ${FT_PREFIX}
      PREFIX "Header Files/Private"
      FILES ${FT_PRIVATE_HEADER_WITH_PREFIX})
    source_group(
      TREE ${FT_PREFIX}
      PREFIX "Source Files"
      FILES ${FT_PRIVATE_SOURCE_WITH_PREFIX})

    source_group(
      TREE ${FT_PREFIX}
      PREFIX "Source Files"
      FILES ${FT_SOURCE_WITH_PREFIX})
  endif()

endfunction()
