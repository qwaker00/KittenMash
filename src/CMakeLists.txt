cmake_minimum_required(VERSION 2.8.12)

project(kittenmash)

find_package(Boost COMPONENTS thread system regex filesystem REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2 -fPIC -flto -Werror")
include_directories(.)

set(SOURCES factories.cpp)
add_subdirectory(orm)
add_subdirectory(image)
add_subdirectory(top)
add_subdirectory(vote)

add_library(api SHARED ${SOURCES})
target_link_libraries(api orm image top vote)
target_link_libraries(api pthread mongoclient boost_thread boost_system boost_regex ssl crypto boost_filesystem ${FCGI_LIBS})

add_executable(rating-updater updater/main.cpp)
target_link_libraries(rating-updater orm pthread mongoclient boost_thread boost_system boost_regex ssl crypto boost_filesystem)

