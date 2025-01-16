set(TEST_SOURCES
    ./unit_tests_matrice.cpp
    ./unit_tests_matrice_1x3.cpp
    ./unit_tests_matrice_4x1.cpp
    ./unit_tests_matrice_1x4.cpp
    ./unit_tests_matrice_3x3.cpp
    ./unit_tests_matrice_4x4.cpp
    ./unit_tests_points.cpp
    ./unit_tests_geometryBase.cpp
    # ./unit_tests_vector.cpp
    ../core/src/Matrix.cpp
    ../core/src/Vector.cpp
    ../core/src/Point.cpp
)
add_executable(unit_tests ${TEST_SOURCES})
target_compile_options(unit_tests PRIVATE --coverage)
target_link_options(unit_tests PRIVATE --coverage)
target_link_libraries(unit_tests PRIVATE ${CRITERION_LIB})
find_library(TBB_LIB tbb)
if(NOT TBB_LIB)
    message(FATAL_ERROR "TBB library not found")
endif()
target_link_libraries(unit_tests PRIVATE ${TBB_LIB})
