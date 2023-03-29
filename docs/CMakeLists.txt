project(TritiumEngine_docs)

if (DOXYGEN_FOUND)
    # Set and configure input/output files
    set(DOXYGEN_IN ${DOCS_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(
        TritiumEngine_docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generate documentation for TritiumEngine API"
        VERBATIM)
else()
    message(WARNING "Doxygen not found, documentation has not been generated.")
endif()