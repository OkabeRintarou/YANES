 cmake_minimum_required ( VERSION 3.6.3 )
 project(yanes CXX)

 set(CMAKE_CXX_STANDARD 17)
 set(CMAKE_CXX_STANDARD_REQUIRED True)

 set(CMAKE_CXX_FLAGS_DEBUG_INIT "-std=c++17 -g -fno-omit-frame-pointer")
 set(CMAKE_CXX_FLAGS_RELEASE_INIT "-std=c++17 -g -O2")

 if (NOT CMAKE_BUILD_TYPE)
	 set(CMAKE_BUILD_TYPE Debug)
 endif()

 add_subdirectory(tests)
