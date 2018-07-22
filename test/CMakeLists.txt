cmake_minimum_required(VERSION 2.8)
project (test_sockman)
AUX_SOURCE_DIRECTORY("." DIR_SRCS)
SET(TEST_MAIN ${DIR_SRCS})
ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_MAIN})
include(CheckFunctionExists)
ADD_DEFINITIONS("-DUSE_DI=2")
include(CheckCXXSourceCompiles)
include(CheckLibraryExists)
include(CPack)
enable_testing()
#add_test(test_main ${PROJECT_NAME})
#set_tests_properties (test_main PROPERTIES PASS_REGULAR_EXPRESSION "PASS!*")  #
add_test(test_pass ${PROJECT_NAME} 3)
set_tests_properties (test_pass PROPERTIES PASS_REGULAR_EXPRESSION "PASS!*")  #
#add_test(test_failed ${PROJECT_NAME} 1)
#set_tests_properties (test_failed PROPERTIES FAIL_REGULAR_EXPRESSION "Error!*")  #
