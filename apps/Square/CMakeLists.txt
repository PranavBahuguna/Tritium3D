add_executable(square Square.cpp)

target_link_libraries(square PRIVATE TritiumEngine glm glfw libglew_static)

# Copy over resouces folder to target
add_custom_target(copy_resouces_square ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${PROJECT_SOURCE_DIR}/Apps/Square/Resources
	${PROJECT_BINARY_DIR}/Apps/Square/Resources
	COMMENT "Copying resources into binary directory")

add_dependencies(square copy_resouces_square)