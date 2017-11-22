add_executable(cli cli.cpp)
target_link_libraries (cli LINK_PUBLIC ${CMAKE_DL_LIBS} hamming)
