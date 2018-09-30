cmake_minimum_required(VERSION 2.6)
project(CAssembler)

#versioning
set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)

#detecting number of bits
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BITS "64 bits")
else()
	set(BITS "32 bits")
endif()

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

#Setting C++ flags
set(CMAKE_CXX_STANDARD 14)
if(MSVC)
	set(CMAKE_CXX_FLAGS "/experimental:external /external:W0 /external:I ${CMAKE_SYSTEM_INCLUDE_PATH} /WX- /Wall /wd4820 /O2")
	add_definitions("/EHsc") #maybe ? WTF, it works !!!
else()
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -O2")
endif()

#Check for Boostd
set(Boost_USE_STATIC_LIBS	ON) #Quick fix
find_package(Boost 1.68 REQUIRED COMPONENTS program_options)

#Main executable
if(Boost_FOUND)
	message(STATUS "found BOOST: " ${BOOST_ROOT})
	include_directories(${Boost_INCLUDE_DIRS})
	add_executable(CAssembler  ${SRC_DIR}/main.cpp ${SRC_DIR}/assembler.cpp ${SRC_DIR}/instructions.cpp  ${SRC_DIR}/regex.cpp  ${SRC_DIR}/opt.cpp ${SRC_DIR}/tools.cpp)
	target_link_libraries(CAssembler ${Boost_LIBRARIES})
else()
	message(FATAL_ERROR "Error: Couldn't find Boost")
endif()
