set(SOURCE_LIB image.cpp)

add_library(image OBJECT ${SOURCE_LIB})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2 -fPIC")

