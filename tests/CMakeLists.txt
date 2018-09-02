

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


add_executable(unit_tests test.cpp)
target_link_libraries(unit_tests bgfgamed ${OpenCV_LIBS} ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})


add_custom_target(run_test
    COMMAND unit_tests
    DEPENDS unit_tests
)



