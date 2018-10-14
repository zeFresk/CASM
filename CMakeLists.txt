cmake_minimum_required(VERSION 2.8.2)

project(CASM)

# add basic assembler to project
add_subdirectory(${PROJECT_SOURCE_DIR}/CAssembler)
include(${PROJECT_SOURCE_DIR}/CAssembler/tests/tests.cmake)

#add_subdirectory(CEmu)

# add unit tests to project
