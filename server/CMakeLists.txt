cmake_minimum_required(VERSION 3.12.1)

project(pdbServer)
set(CMAKE_CXX_STANDARD 11)

# Create main target
add_executable(pdbServer ${GUI_FLAG} "")
target_sources(pdbServer
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/PDBAudiobook.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/PDBAudiobook.h"
        "${CMAKE_CURRENT_LIST_DIR}/src/PDBCore.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/PDBCore.h"
)
target_include_directories(pdbServer PUBLIC "src/")

cmake_minimum_required(VERSION 3.12.1)