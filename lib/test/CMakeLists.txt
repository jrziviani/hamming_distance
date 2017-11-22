include_directories(../include)
add_executable(unit_test unit_tests.cpp ../src/hamming_distance.cpp)
add_executable(create_cases create_test_cases.cpp)
