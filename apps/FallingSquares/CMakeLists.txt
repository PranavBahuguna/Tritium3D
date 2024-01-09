file(GLOB_RECURSE FALLING_SQUARES_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_executable(FallingSquares ${FALLING_SQUARES_FILES})

target_link_libraries(FallingSquares
  PRIVATE TritiumEngine glm glfw libglew_static EnTT)

copy_resources(FallingSquares)