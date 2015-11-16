cmake_minimum_required(VERSION 2.8.12)

project(kittenmash)
include_directories(.)
add_subdirectory(orm)
add_subdirectory(image)

add_library(api SHARED $<TARGET_OBJECTS:orm> $<TARGET_OBJECTS:image>)
target_link_libraries(api pthread mongoclient boost_thread boost_system boost_regex ssl crypto boost_filesystem ${FCGI_LIBS})

