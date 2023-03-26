project(Tritium3D_docs)

if (DOXYGEN_FOUND)
    add_custom_target(
        Tritium3D_docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOCS_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generate documentation for Tritium3D API"
        VERBATIM)
else()
    message(WARNING "Doxygen not found, documentation has not been generated.")
endif()