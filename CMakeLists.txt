cmake_minimum_required(VERSION 2.6)
project(CAssembler)

#versioning
set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)

#Boost needed
find_package(Boost REQUIRED program_options)
include_directories(${Boost_INCLUDE_DIRS})

#adding includes and sources
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
include_directories("${INCLUDE_DIR}")
file(GLOB SOURCES "src/*.cpp")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

#configuration header
configure_file(
	"${SRC_DIR}/CMake_config.h.in"
	"${INCLUDE_DIR}/CMake_config.h"
	)

add_executable(CAssembler ${SOURCES})

target_link_libraries(CAssembler LINK_PUBLIC ${Boost_LIBRARIES})
