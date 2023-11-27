# Project settings
cmake_minimum_required(VERSION 3.8)
project(TritiumEngine LANGUAGES CXX)

# C++ settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build options
option(GIT_SUBMODULE "Check submodules during build" ON)
option(TRITIUM_BUILD_APPS "Build example applications" ON)
option(TRITIUM_BUILD_DOCS "Generate documentation" ON)

# Directories
set(TRITIUM_INC_DIR ${PROJECT_SOURCE_DIR}/inc)
set(TRITIUM_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(APPS_DIR ${PROJECT_SOURCE_DIR}/apps)
set(DOCS_DIR ${PROJECT_SOURCE_DIR}/docs)
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty)

# Automatically updates submodules during a build
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS ${PROJECT_SOURCE_DIR}/.git)
    if (GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL 0)
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules.")
        endif()
    endif()
endif()

# Check if submodules were downloaded
file(GLOB THIRDPARTY_FILES ${THIRDPARTY_DIR})
list(LENGTH THIRDPARTY_FILES DIR_SIZE)
if (DIR_SIZE EQUAL 0)
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

# Init settings for thirdparty modules
add_definitions(-DGLEW_STATIC)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add header-only libraries
add_library(thirdparty_headers_lib INTERFACE)
target_include_directories(thirdparty_headers_lib INTERFACE ${THIRDPARTY_DIR}/headers)

# Add all 3rd party modules
add_subdirectory(${THIRDPARTY_DIR}/glew)
add_subdirectory(${THIRDPARTY_DIR}/glfw)
add_subdirectory(${THIRDPARTY_DIR}/glm)
add_subdirectory(${THIRDPARTY_DIR}/entt)

# Add generation of documentation
if (TRITIUM_BUILD_DOCS)
    find_package(Doxygen)
    add_subdirectory(${DOCS_DIR})
endif()

# Setup TritiumEngine project library
file(GLOB_RECURSE TRITIUM_INC_FILES "${TRITIUM_INC_DIR}/*.hpp")
file(GLOB_RECURSE TRITIUM_SRC_FILES "${TRITIUM_SRC_DIR}/*.cpp")

add_library(TritiumEngine ${TRITIUM_SRC_FILES} ${TRITIUM_INC_FILES})

target_include_directories(TritiumEngine PUBLIC ${TRITIUM_INC_DIR})

target_link_libraries(TritiumEngine
    PUBLIC thirdparty_headers_lib
    PRIVATE glm glfw libglew_static EnTT)

# Add example apps
if (TRITIUM_BUILD_APPS)
    add_subdirectory(${APPS_DIR})
endif()