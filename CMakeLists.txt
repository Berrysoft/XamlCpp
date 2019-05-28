cmake_minimum_required(VERSION 3.1)
project(XamlCpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include_directories(include)

function(add_headers VAR)
  set(headers ${${VAR}})
  foreach (header ${ARGN})
    set(headers ${headers} include/xaml/${header})
  endforeach()
  set(${VAR} ${headers} PARENT_SCOPE)
endfunction()

add_headers(SF_HEADERS event.hpp conv.hpp)

install(FILES ${SF_HEADERS} DESTINATION include/xaml)

file(GLOB TEST_SOURCE "test/*.cpp")
add_executable(test ${TEST_SOURCE})
