# Directories
set(RENDERING_BENCHMARK_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(RENDERING_BENCHMARK_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE RENDERING_BENCHMARK_FILES "${RENDERING_BENCHMARK_SRC_DIR}/*.cpp" "${RENDERING_BENCHMARK_INC_DIR}/*.hpp")

add_executable(RenderingBenchmark ${RENDERING_BENCHMARK_FILES})

target_include_directories(RenderingBenchmark PUBLIC ${RENDERING_BENCHMARK_INC_DIR})

target_link_libraries(RenderingBenchmark
  PRIVATE TritiumEngine glm glfw libglew_static EnTT)

copy_resources(RenderingBenchmark)