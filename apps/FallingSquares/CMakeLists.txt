add_executable(FallingSquares Main.cpp)

target_link_libraries(FallingSquares PRIVATE TritiumEngine glm glfw libglew_static EnTT)

copy_resources(FallingSquares)