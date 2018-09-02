
add_executable(demo demo.cpp)
target_link_libraries( demo bgfgamed ${OpenCV_LIBS} )

add_custom_target(run
    COMMAND demo
    DEPENDS demo
)
