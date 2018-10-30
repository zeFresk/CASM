set(CASSEMBLER_DIR ${PROJECT_SOURCE_DIR})
set(TEST_DIR ${CASSEMBLER_DIR}/tests)
# Will download and compile googletest
add_subdirectory(${CASSEMBLER_DIR}/../third_party/googletest ${CMAKE_CURRENT_BINARY_DIR}/third_party/googletest) #need to be tricky here

enable_testing()

file(GLOB TEST_FILES "${TEST_DIR}/src/*.cpp")

add_executable(CAssembler_tests ${TEST_DIR}/main_tests.cpp ${TEST_FILES})
target_include_directories(CAssembler_tests PUBLIC ${CASSEMBLER_DIR}/include)

target_link_libraries(CAssembler_tests CAssembler_lib GTest::GTest GTest::Main)

# We need to copy the test files
add_custom_command(TARGET CAssembler_tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEST_DIR}/asm $<TARGET_FILE_DIR:CAssembler_tests>/asm)

add_test(CAssembler_all CAssembler_tests)
#### set up c++ flags
set_property(TARGET CAssembler_tests PROPERTY CXX_STANDARD 14)

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
