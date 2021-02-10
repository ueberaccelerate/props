find_package(yaml-cpp 0.6.2 CONFIG REQUIRED)

add_target(
  TARGET
  propertysdk
  PREFIX
  ${PROPERTYSDK_SOURCE_DIR}/src
  PUBLIC_PREFIX
  ${PROPERTYSDK_SOURCE_DIR}/include
  PUBLIC_HEADER
  ${PROPERTYSDK_NAME}/property.hpp
  ${PROPERTYSDK_NAME}/property_config.hpp
  ${PROPERTYSDK_NAME}/serialize_node.hpp
  ${PROPERTYSDK_NAME}/serializable.hpp
  ${PROPERTYSDK_NAME}/serializable_holder.hpp
  ${PROPERTYSDK_NAME}/serializable_scalar_holder.hpp
  ${PROPERTYSDK_NAME}/serializable_sequence_holder.hpp
  ${PROPERTYSDK_NAME}/serializable_map_holder.hpp
  ${PROPERTYSDK_NAME}/serializable_serialize_holder.hpp
  SOURCE
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
