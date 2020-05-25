cmake_minimum_required(VERSION 3.12)
project (propertysdk)

option( BUILD_SHARED_LIBS "The shared library is made" OFF )


include ( cmake/warnings.cmake )
include ( cmake/dependency.cmake )

add_library( propertysdk-cppfeatrues INTERFACE )
add_library( propertysdk-cppwarnings INTERFACE )

target_compile_features( propertysdk-cppfeatrues INTERFACE cxx_std_17 )

target_include_directories( propertysdk-cppfeatrues INTERFACE include )

set_project_warinigs( propertysdk-cppwarnings )

add_library( propertysdk 

# $(CMAKE_CURRENT_SOURCE_DIR}/include/property/
  main.cpp
)
target_link_libraries( propertysdk PRIVATE propertysdk-cppfeatrues propertysdk-cppwarnings ueberlog)

#add_subdirectory(ueberlog)
#
##add_subdirectory(yaml-cpp)
#add_executable( yaml-prop main.cpp)
#target_compile_features(yaml-prop PUBLIC cxx_std_17)
#target_link_libraries(yaml-prop PUBLIC ueberlog)
