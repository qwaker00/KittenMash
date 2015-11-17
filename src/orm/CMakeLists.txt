set(SOURCE_LIB db.cpp db_impl.cpp image.cpp rating.cpp vote.cpp)

add_library(orm STATIC ${SOURCE_LIB})
