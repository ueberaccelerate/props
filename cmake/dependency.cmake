include ( cmake/CPM.cmake )

CPMAddPackage(
  NAME ueberlog
  GITHUB_REPOSITORY ueberaccelerate/ueberlog
  GIT_TAG master
)

CPMAddPackage(
  NAME yaml-cpp
  GITHUB_REPOSITORY jbeder/yaml-cpp
  # 0.6.2 uses deprecated CMake syntax
  VERSION 0.6.3
  # 0.6.3 is not released yet, so use a recent commit
  GIT_TAG 012269756149ae99745b6dafefd415843d7420bb 
  OPTIONS
  "YAML_CPP_INSTALL OFF"
  "YAML_CPP_BUILD_TESTS OFF"
  "YAML_CPP_BUILD_CONTRIB OFF"
  "YAML_CPP_BUILD_TOOLS OFF"
)



function ( set_dependency target_name target_type ) 

target_link_libraries( ${target_name} ${target_type} ueberlog yaml-cpp)

endfunction()
