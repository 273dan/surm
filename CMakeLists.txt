cmake_minimum_required(VERSION 4.1)
project(surm)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Configure yaml-cpp
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Disable yaml-cpp tests")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Disable yaml-cpp tools")
add_subdirectory(third_party/yaml-cpp)

add_executable(surm)
target_link_libraries(surm PRIVATE yaml-cpp)
target_include_directories(surm PRIVATE 
    third_party/rang/include
    third_party/indicators/include
)

add_subdirectory(src)


