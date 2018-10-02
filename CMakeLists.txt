cmake_minimum_required(VERSION 2.8.2) #We need to be able to download things

project(CAssembler)


#versioning
set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)

#adding includes and sources
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
include_directories("${INCLUDE_DIR}")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
file(GLOB SOURCES "${SRC_DIR}/*.cpp")

add_executable(CAssembler  ${SOURCES})

#options
option(BUILD_TESTS "Build Google tests" OFF)

#setting up tests if needed
if(BUILD_TESTS)
	include(CTest) #for automatized testing

	set(TEST_DIR "${PROJECT_SOURCE_DIR}/tests")	#searching for gtest

	find_package(GTest)
	if(GTEST_FOUND)
		set(GTEST_LIBRARIES ${GTEST_BOTH_LIBRARIES})
		set(GTEST_INCLUDE_DIR ${GTEST_INCLUDE_DIRS})

	else() #no Google test found, building our own
		message(STATUS "No Google Test found, building our own version...")

		#Download/Build/Link Googlr test
		#All credits go to Crascit
		if(CMAKE_VERSION VERSION_LESS 3.2)
			set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
		else()
			set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
		endif()
		
		include(${TEST_DIR}/GTest/DownloadProject.cmake)

		message(STATUS "Downloading GTest")
		download_project(PROJ                googletest
				 PREFIX              ${TEST_DIR}/GTest 
				 GIT_REPOSITORY      https://github.com/google/googletest.git
				 GIT_TAG             master
				 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
				 QUIET
		)

		# Prevent GoogleTest from overriding our compiler/linker options
		# when building with Visual Studio
		set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

		add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

		# When using CMake 2.8.11 or later, header path dependencies
		# are automatically added to the gtest and gmock targets.
		# For earlier CMake versions, we have to explicitly add the
		# required directories to the header search path ourselves.
		if (CMAKE_VERSION VERSION_LESS 2.8.11)
		    include_directories("${gtest_SOURCE_DIR}/include"
					"${gmock_SOURCE_DIR}/include")
		endif()

		target_link_libraries(CAssembler gtest gmock_main)
	endif()
else()
	message(WARNING "Building without tests")
endif()

#detecting number of bits
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BITS "64 bits")
else()
	set(BITS "32 bits")
endif()

#configuration header
configure_file(
	"${SRC_DIR}/CMake_config.h.in"
	"${INCLUDE_DIR}/CMake_config.h"
	)

#Setting C++ flags

set_property(TARGET CAssembler PROPERTY CXX_STANDARD 14)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX- /Wall /wd4820 /O2")

	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.14) #disable external warning
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /experimental:external /external:W0 ")
		set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "/external:I ")
	endif()

	add_definitions("/EHsc") #maybe ? WTF, it works !!! (Fix Boost link error to exceptions)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O2")
endif()

#Check for Boost
set(Boost_USE_STATIC_LIBS	ON) #Quick fix
find_package(Boost REQUIRED COMPONENTS program_options)

#Link and include Boost
if(Boost_FOUND)
	message(STATUS "found BOOST: " ${BOOST_ROOT})
	include_directories(${Boost_INCLUDE_DIRS})
	target_link_libraries(CAssembler ${Boost_LIBRARIES})
else()
	message(FATAL_ERROR "Error: Couldn't find Boost")
endif()
