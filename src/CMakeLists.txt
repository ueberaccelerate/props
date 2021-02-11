find_package(yaml-cpp 0.6.2 CONFIG REQUIRED)

add_target(
  TARGET
  ${PROPERTYSDK_NAME}
  VERSION
  ${PROPERTYSDK_VERSION}
  EXPORTED
  PREFIX
  ${PROPERTYSDK_SOURCE_DIR}/src
  PUBLIC_PREFIX
  ${PROPERTYSDK_SOURCE_DIR}/include
  PUBLIC_HEADER
  ${PROPERTYSDK_NAME}/property.hpp
  ${PROPERTYSDK_NAME}/property_config.hpp
  ${PROPERTYSDK_NAME}/version.hpp
  ${PROPERTYSDK_NAME}/serialize_node.hpp
  ${PROPERTYSDK_NAME}/serializable.hpp
  ${PROPERTYSDK_NAME}/serializable_holder.hpp
  ${PROPERTYSDK_NAME}/serializable_scalar_holder.hpp
  ${PROPERTYSDK_NAME}/serializable_sequence_holder.hpp
  ${PROPERTYSDK_NAME}/serializable_map_holder.hpp
  ${PROPERTYSDK_NAME}/serializable_serialize_holder.hpp
  SOURCES
  serialize_node.cpp
  property.cpp
  PUBLIC_LINKS
  yaml-cpp
  BINARY_DIRECTORY
  ${PROPERTYSDK_OUTPUT_DIR})

target_compile_features(propertysdk PUBLIC cxx_std_17)

if(CLANG_TIDY_FOUND)
  clang_tidy(TARGET ${PROPERTYSDK_NAME})

  if(PROPERTYSDK_AUTO_CLANG_TIDY)
    add_dependencies(${PROPERTYSDK_NAME} tidy_${PROPERTYSDK_NAME})
  endif()
endif()

if(PROPERTYSDK_AUTO_CLANG_FORMAT AND CLANG_FORMAT)
  add_dependencies(${PROPERTYSDK_NAME} format)
endif()



# install headers
include(CMakePackageConfigHelpers)

configure_package_config_file( 
  "${PROPERTYSDK_SOURCE_DIR}/cmake/${PROPERTYSDK_NAME}.cmake.in" 
  "${PROPERTYSDK_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROPERTYSDK_NAME})
  
   
install(
  DIRECTORY ${PROPERTYSDK_SOURCE_DIR}/include/${PROPERTYSDK_NAME}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN "*.hpp")
# install targets


install(
  EXPORT ${PROJECT_NAME}-targets
  NAMESPACE ${PROPERTYSDK_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROPERTYSDK_NAME}
  FILE ${PROPERTYSDK_NAME}-targets.cmake)
  
# Install Config and ConfigVersion files
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROPERTYSDK_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROPERTYSDK_NAME}ConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROPERTYSDK_NAME}"
)



write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROPERTYSDK_NAME}ConfigVersion.cmake
  VERSION ${PROPERTYSDK_VERSION}
  COMPATIBILITY SameMajorVersion
  )
  
# this exports the targets only to be used for the in-tree compilation, do not export this
export(EXPORT ${PROJECT_NAME}-targets FILE "${CMAKE_INSTALL_LIBDIR}/cmake/${PROPERTYSDK_NAME}/${PROPERTYSDK_NAME}-targets.cmake")
