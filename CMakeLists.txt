cmake_minimum_required(VERSION 3.13)
project(ademma CXX)

#file(GLOB CORE_SOURCES core_src/)
set(CORE_SOURCES
	core_src/math.cpp
	core_src/steenrod_square.cpp
	core_src/classical_adem_monomial.cpp
	core_src/classical_adem_polynomial.cpp
)
add_executable(${PROJECT_NAME} ${CORE_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC
	core_include
)

