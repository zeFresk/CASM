set(CASSEMBLER_DIR ${PROJECT_SOURCE_DIR}/CAssembler)
set(TEST_DIR ${PROJECT_SOURCE_DIR}/CAssembler/tests)
# Will download and compile googletest
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/googletest)

enable_testing()

file(GLOB TEST_FILES "${TEST_DIR}/src/*.cpp")
file(GLOB LIB_FILES "${CASSEMBLER_DIR}/src/*.cpp")

add_executable(CAssembler_tests ${TEST_DIR}/src/CAssembler_tests.cpp ${TEST_FILES})
target_include_directories(CAssembler_tests PUBLIC ${CASSEMBLER_DIR}/include)

add_library(CAssembler_lib ${LIB_FILES})
target_include_directories(CAssembler_lib PUBLIC ${CASSEMBLER_DIR}/include)

#Check for Boost
if(MSVC)
	set(Boost_USE_STATIC_LIBS	ON) #Quick fix
endif()
find_package(Boost REQUIRED COMPONENTS program_options)

#Link and include Boost
if(Boost_FOUND)
	#message(STATUS "found BOOST: " ${BOOST_ROOT})
	#message(STATUS "boost include: " ${Boost_INCLUDE_DIRS})
	target_include_directories(CAssembler_lib SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
	target_link_libraries(CAssembler_lib ${Boost_LIBRARIES})
else()
	message(FATAL_ERROR "Error: Couldn't find Boost")
endif()

target_link_libraries(CAssembler_tests CAssembler_lib GTest::GTest GTest::Main)

add_test(CAssembler_all CAssembler_tests)
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
