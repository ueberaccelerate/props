cmake_minimum_required(VERSION 3.2)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake") 
project (example)

add_subdirectory(ueberlog)

#add_subdirectory(yaml-cpp)
add_executable( yaml-prop main.cpp)
target_compile_features(yaml-prop PUBLIC cxx_std_17)
target_link_libraries(yaml-prop PUBLIC ueberlog)
