cmake_minimum_required(VERSION 2.8.2)

project(CAssembler)

#versioning
set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)

#setting includes and sources
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
file(GLOB SOURCES "${SRC_DIR}/*.cpp")

#creating library
add_library(CAssembler_lib ${SOURCES})
target_include_directories(CAssembler_lib PUBLIC ${INCLUDE_DIR})

#creating main executable
add_executable(CAssembler "main.cpp")
target_include_directories(CAssembler PUBLIC ${INCLUDE_DIR})
target_link_libraries(CAssembler CAssembler_lib)

#adding tests if set
option(BUILD_TESTS "Build tets and googletest" OFF) #off by default to not overload user
if (BUILD_TESTS)
	include(tests/tests.cmake)
else()
	message(AUTHOR_WARNING "Not compiling tests.")
endif()

#Setting C++ flags
set_property(TARGET CAssembler_lib CAssembler PROPERTY CXX_STANDARD 14)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX- /Wall /wd4514 /wd4820 /O2")

	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.14) #disable external warning
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /experimental:external /external:W0 ")
		set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "/external:I ")
	endif()

	add_definitions("/EHsc") #maybe ? WTF, it works !!! (Fix Boost link error to exceptions)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O2")
endif()

#Check for Boost
if(MSVC)
	set(Boost_USE_STATIC_LIBS	ON) #Quick fix
endif()
find_package(Boost REQUIRED COMPONENTS program_options)

#Link and include Boost
if(Boost_FOUND)
	target_include_directories(CAssembler SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
	#target_link_libraries(CAssembler ${Boost_LIBRARIES})
	target_include_directories(CAssembler_lib SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
	target_link_libraries(CAssembler_lib ${Boost_LIBRARIES})

else()
	message(FATAL_ERROR "Error: Couldn't find Boost.")
endif()

#configuration header
#detecting number of bits
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BITS 64)
else()
	set(BITS 32)
endif()

configure_file(
	"${SRC_DIR}/CMake_config.h.in"
	"${INCLUDE_DIR}/CMake_config.h"
	)
