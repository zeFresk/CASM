cmake_minimum_required(VERSION 2.8.2)

project(tests)

include_directories(../third_party/google-test)
include_directories(../CEmu/include)
include_directories(./include)

add_executable(CAssembler_tests ./src/CAssembler_tests.cpp)

#### set up c++ flags
set_property(TARGET CAssembler PROPERTY CXX_STANDARD 14)

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

#### add google-test
add_subdirectory(third_party/google-test)

# enable CTest testing
enable_testing()

#### CAssembler tests
include_directories(../CAssembler/include)

file(GLOB SOURCES "../CAssembler/src/*.cpp")

add_library(CAssembler_lib ${SOURCES})

target_link_libraries(CAssembler_tests CAssembler_lib gtest gtest_main)

add_test(CAssembler_all_test CAssembler_tests)
