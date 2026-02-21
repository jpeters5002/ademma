cmake_minimum_required(VERSION 3.13)
project(ademma CXX)

set(CORE_SOURCES
	core_src/steenrod_square.cpp
	core_src/classical_adem_math.cpp
	core_src/classical_adem_monomial.cpp
	core_src/classical_adem_polynomial.cpp
)
set(TUI_SOURCES
    tui_src/main.cpp
)
add_executable(${PROJECT_NAME} ${TUI_SOURCES} ${CORE_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC
	core_include
)

