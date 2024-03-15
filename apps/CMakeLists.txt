# Function for creating a symlink to source Resources folder
function(link_resources APP_NAME)
    add_custom_target(link_target ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${PROJECT_SOURCE_DIR}/Apps/${APP_NAME}/Resources
        ${PROJECT_BINARY_DIR}/Apps/${APP_NAME}/Resources
        COMMENT "Linking resources from binary to source directory")
endfunction()

# Add example applications
add_subdirectory(${APPS_DIR}/RenderingBenchmark)