cmake_minimum_required(VERSION 3.12)
project (propertysdk)

option( BUILD_SHARED_LIBS "The shared library is made" OFF )


include ( cmake/warnings.cmake )
include ( cmake/dependency.cmake )

add_library( propertysdk-cppfeatrues INTERFACE )
add_library( propertysdk-cppwarnings INTERFACE )

target_compile_features( propertysdk-cppfeatrues INTERFACE cxx_std_17 )


set_project_warinigs( propertysdk-cppwarnings )

add_library( propertysdk )

set_dependency( propertysdk PUBLIC )

add_subdirectory( src )

target_include_directories( propertysdk PUBLIC src/include )
target_link_libraries( propertysdk PUBLIC propertysdk-cppfeatrues PRIVATE propertysdk-cppwarnings )

# examples
add_executable( propertysdk-example main.cpp)
target_link_libraries( propertysdk-example PRIVATE propertysdk )
