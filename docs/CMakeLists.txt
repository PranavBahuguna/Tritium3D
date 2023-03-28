project(TritiumEngine_docs)

if (DOXYGEN_FOUND)
    add_custom_target(
        TritiumEngine_docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOCS_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generate documentation for TritiumEngine API"
        VERBATIM)
else()
    message(WARNING "Doxygen not found, documentation has not been generated.")
endif()