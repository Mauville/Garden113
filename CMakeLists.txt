cmake_minimum_required(VERSION 3.21)
project(garden113)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(garden113
    Comms.ino
    garden113.ino
    Time.ino
    Utils.ino)
