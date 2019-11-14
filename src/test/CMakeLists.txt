cmake_minimum_required(VERSION 3.5)
project(log)

macro(source_group_by_dir source_files)
    set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(sgbd_file ${${source_files}})
        string(REGEX REPLACE ${sgbd_cur_dir}//(.*/) //1 sgbd_fpath ${sgbd_file})
        string(REGEX REPLACE "/(.*/)/.*" //1 sgbd_group_name ${sgbd_fpath})
        string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
        string(REPLACE "/" "//" sgbd_group_name ${sgbd_group_name})
        if(sgbd_nogroup)
            set(sgbd_group_name "//")
        endif(sgbd_nogroup)
        source_group(${sgbd_group_name} FILES ${sgbd_file})
    endforeach(sgbd_file)
endmacro(source_group_by_dir)

add_definitions("-g")

FILE(GLOB_RECURSE all_files
        *.cpp
        )
link_directories("$ENV{HOME}/github/ultimate/lib/")

source_group(all_files)
include_directories($ENV{HOME}/github/ultimate/include/log)
include_directories($ENV{HOME}/github/ultimate/include/)
include_directories($ENV{HOME}/github/ultimate/3rdparty/include/)
include_directories($ENV{HOME}/github/ultimate/3rdparty/include/google/protobuf/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(mainTest main.cpp)
add_executable(pubTest pubTest.cpp)
add_executable(subTest subTest.cpp)

target_link_libraries(mainTest log pubsub protobuf)
target_link_libraries(pubTest pubsub protobuf)
target_link_libraries(subTest pubsub protobuf)
