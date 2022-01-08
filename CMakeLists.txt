cmake_minimum_required(VERSION 3.16)
project(FCCS-Tutorials) 
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(FCCS)

add_subdirectory(01-CreateWindow)
add_subdirectory(02-InitializeDevice)
add_subdirectory(03-DrawTriangle)
add_subdirectory(04-PostProcess)
