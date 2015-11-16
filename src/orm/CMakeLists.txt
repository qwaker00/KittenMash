set(SOURCE_LIB db.cpp db_impl.cpp image.cpp)

add_library(orm OBJECT ${SOURCE_LIB})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2 -fPIC")

