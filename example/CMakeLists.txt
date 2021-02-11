
add_target(
  TARGET
    propertysdk_example
  SOURCES
    main.cpp
  PRIVATE_LINKS
    propertysdk
  BINARY_DIRECTORY
    ${PROPERTYSDK_OUTPUT_DIR}
  EXECUTABLE
)

