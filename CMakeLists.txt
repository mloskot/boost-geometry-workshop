cmake_minimum_required(VERSION 2.8)
project(boost_geometry_workshop)

find_package(Boost 1.44 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif() 
message(STATUS "Using Boost headers from ${Boost_INCLUDE_DIR}")

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-D_SCL_SECURE_NO_DEPRECATE)
endif()

add_executable(boost_geometry_wkb boost_geometry_wkb.cpp)
add_executable(boost_geometry_any boost_geometry_any.cpp)
add_executable(boost_geometry_reproduce_msvc_using_bug boost_geometry_reproduce_msvc_using_bug.cpp)