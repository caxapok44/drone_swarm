cmake_minimum_required(VERSION 3.20)


add_executable(main_app
    src/main.cpp
    src/CLIOptions.cpp
    src/GridHandler.cpp
    src/GridAlgo.cpp
    src/GridFileLoader.cpp
)

target_include_directories(main_app
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR} 
)