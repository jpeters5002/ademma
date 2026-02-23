cmake_minimum_required(VERSION 3.13)
project(ademma VERSION 0.0.1 LANGUAGES CXX)

set(CORE_SOURCES
	core_src/classical_adem_math.cpp
	core_src/classical_adem_monomial.cpp
	core_src/classical_adem_polynomial.cpp
	core_src/general_adem_math.cpp
	core_src/parsing_info.cpp
	core_src/r_motivic_adem_math.cpp
	core_src/r_motivic_adem_monomial.cpp
	core_src/r_motivic_adem_polynomial.cpp
	core_src/steenrod_square.cpp
)
set(TUI_SOURCES
    tui_src/main.cpp
)
add_executable(${PROJECT_NAME} ${TUI_SOURCES} ${CORE_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC
	core_include
)

