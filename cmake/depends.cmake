get_property(DEPENDS_INITIALIZED GLOBAL "" PROPERTY DEPENDS_INITIALIZED SET)
if (DEPENDS_INITIALIZED)
  return()
endif()

set_property(GLOBAL PROPERTY DEPENDS_INITIALIZED true)

include(get_cpm)
include(CMakeParseArguments)

function ( add_git_dependency )
  set(oneValueArgs
    TARGET
    TARGET_ACCESS
    OUTPUT_DIRECTORY
    NAME
    GIT_TAG
    GITHUB_REPOSITORY
    GITLAB_REPOSITORY
    GIT_REPOSITORY
    GIT_SHALLOW
  )

  set(multiValueArgs
    OPTIONS
    IMPORT_LIBS
  )
  cmake_parse_arguments(DEPENDS "" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}")
  
  if(NOT DEPENDS_TARGET)
    message(FATAL_ERROR "specify target name.")
  endif()
  if(NOT DEPENDS_OUTPUT_DIRECTORY)
    set(DEPENDS_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  endif()
  
  if(DEPENDS_GITHUB_REPOSITORY AND NOT DEPENDS_GIT_SELECTED)
    set(GIT_SELECTED ${DEPENDS_GITHUB_REPOSITORY})
    set(GIT_SELECTED_REPOSITORY GITHUB_REPOSITORY)
    set(GIT_SELECTED_FLAG TRUE)
  endif()
  if(DEPENDS_GITLAB_REPOSITORY AND NOT GIT_SELECTED)
    set(GIT_SELECTED ${DEPENDS_GITLAB_REPOSITORY})
    set(GIT_SELECTED_REPOSITORY GITLAB_REPOSITORY)
    set(GIT_SELECTED_FLAG TRUE)
  endif() 
  if(DEPENDS_GIT_REPOSITORY AND NOT GIT_SELECTED)
    set(GIT_SELECTED ${DEPENDS_GIT_REPOSITORY})
    set(GIT_SELECTED_REPOSITORY GIT_REPOSITORY)
    set(GIT_SELECTED_FLAG TRUE)
  endif() 
  
  if(NOT GIT_SELECTED_FLAG)
    message(FATAL_ERROR "Use one of ( GITHUB_REPOSITORY | GITLAB_REPOSITORY | GIT_REPOSITORY )")
  endif()
  if(NOT DEPENDS_TARGET_TYPE)
    set(DEPENDS_TARGET_TYPE PUBLIC)
  endif()
  
  cpmfindpackage (
    NAME  
      ${DEPENDS_NAME}
    ${GIT_SELECTED_REPOSITORY}
      ${GIT_SELECTED}
    GIT_TAG
      ${DEPENDS_GIT_TAG}
    GIT_SHALLOW
      ${DEPENDS_GIT_SHALLOW}
    OPTIONS
      ${DEPENDS_OPTIONS}
  )
  
  set_target_properties(easy_profiler PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${DEPENDS_OUTPUT_DIRECTORY}"
                                                 RUNTIME_OUTPUT_DIRECTORY "${DEPENDS_OUTPUT_DIRECTORY}")
                                                 
  target_link_libraries(${DEPENDS_TARGET} ${DEPENDS_TARGET_ACCESS} ${DEPENDS_IMPORT_LIBS})
endfunction()
  
  