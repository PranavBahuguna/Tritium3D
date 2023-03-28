add_executable(triangle Triangle.cpp)

target_link_libraries(triangle PRIVATE TritiumEngine glm glfw libglew_static)

# Copy over resouces folder to target
add_custom_target(copy_resouces_triangle ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${PROJECT_SOURCE_DIR}/Apps/Triangle/Resources
	${PROJECT_BINARY_DIR}/Apps/Triangle/Resources
	COMMENT "Copying resources into binary directory")

add_dependencies(triangle copy_resouces_triangle)