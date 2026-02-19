cmake_minimum_required(VERSION 3.13)
project(ademma CXX)

#file(GLOB CORE_SOURCES core_src/)
set(CORE_SOURCES
	core_src/math.cpp
)
add_executable(${PROJECT_NAME} ${CORE_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC
	core_include
)

