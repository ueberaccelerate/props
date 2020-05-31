

target_sources( propertysdk PRIVATE
  # headers
  ${CMAKE_CURRENT_SOURCE_DIR}/include/property/property.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/property/property_config.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/property/serialize_node.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/property/serializable.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/property/serializable_holder.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/property/serializable_scalar_holder.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/property/serializable_sequence_holder.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/property/serializable_serialize_holder.hpp


  ${CMAKE_CURRENT_SOURCE_DIR}/serialize_node.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/property.cpp
)
