cmake_minimum_required(VERSION 3.20)

project(drone_swarm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(app)

include(CTest)
if (BUILD_TESTING)
  add_subdirectory(tests)
endif()


