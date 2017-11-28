set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

add_executable(lzw435
	lzw.cpp)

add_executable(lzw435M
	lzw2.cpp)



