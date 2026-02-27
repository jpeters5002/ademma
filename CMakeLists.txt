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

option(BUILD_TESTING "Build the project's tests" ON)

if (BUILD_TESTING)
    set(TEST_PROJECT_NAME test_${PROJECT_NAME})
    set(TEST_SOURCES
        test_src/main.cpp
        test_src/testing_helper_functions.cpp
        test_src/test_cl_prod_binary_degrees_1_to_n.cpp
        test_src/test_sq1_sq2n.cpp
    )
    add_executable(${TEST_PROJECT_NAME} ${TEST_SOURCES} ${CORE_SOURCES})
    target_include_directories(${TEST_PROJECT_NAME} PUBLIC
        test_include
        core_include
    )
endif()

