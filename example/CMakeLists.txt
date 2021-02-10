
add_target(
  TARGET
  propertysdk_example
  SOURCE
  main.cpp
  PRIVATE_LINKS
  propertysdk
  BINARY_DIRECTORY
  ${PROPERTYSDK_OUTPUT_DIR}
  EXECUTABLE)
