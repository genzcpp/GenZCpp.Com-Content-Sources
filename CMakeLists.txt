cmake_minimum_required(VERSION 3.20.0)
project(GenzCpp CXX)

option(filename "file_name" NOTFOUND)

message(${filename})

find_package(Boost 1.80.0)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(main ${filename}.cpp)

set_property(TARGET main PROPERTY CXX_STANDARD 20)