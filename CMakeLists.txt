cmake_minimum_required(VERSION 2.8.2)

project(CASM)

#subprojects
add_subdirectory(CAssembler)

#add_subdirectory(CEmu)

add_subdirectory(tests/third_party/google-test)

enable_testing()

add_executable(unit_tests tests/src/CAssembler_tests.cpp)
add_library(tst CAssembler/src/tools.cpp)
target_include_directories(tst PUBLIC CAssembler/include)

target_include_directories(unit_tests PUBLIC CAssembler/include)
target_link_libraries(unit_tests tst GTest::GTest GTest::Main)

add_test(test_all unit_tests)
