# Function for copying resources over to build
function(copy_resources APP_NAME)
    set(COPY_RESOURCES_TARGET copy_resouces_${APP_NAME})
    add_custom_target(${COPY_RESOURCES_TARGET} ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/Apps/${APP_NAME}/Resources
        ${PROJECT_BINARY_DIR}/Apps/${APP_NAME}/Resources
        COMMENT "Copying resources into binary directory")

    add_dependencies(${APP_NAME} ${COPY_RESOURCES_TARGET})
endfunction()

# Add example applications
add_subdirectory(${APPS_DIR}/FallingSquares)