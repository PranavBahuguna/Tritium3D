add_executable(FallingSquares Main.cpp Square.hpp Square.cpp)

target_link_libraries(FallingSquares PRIVATE TritiumEngine glm glfw libglew_static)

copy_resources(FallingSquares)