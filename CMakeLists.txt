cmake_minimum_required(VERSION 3.14)
project(CCSL VERSION 0.1.0
        DESCRIPTION "Chandra Credit Software License System"
        LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to Release build
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Option to build examples
option(CCSL_BUILD_EXAMPLES "Build CCSL examples" ON)
# Option to build tests
option(CCSL_BUILD_TESTS "Build CCSL tests" ON)
# Option to build documentation
option(CCSL_BUILD_DOCS "Build CCSL documentation" OFF)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# Collect source files
file(GLOB_RECURSE CCSL_SOURCES
    "src/ccsl/*.cpp"
)

# Create the CCSL library
add_library(ccsl STATIC ${CCSL_SOURCES})
target_include_directories(ccsl PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
set_target_properties(ccsl PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Build the code checker utility
add_executable(code-checker code-checker.c)

# Add examples subdirectory if enabled
if(CCSL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Add tests subdirectory if enabled
if(CCSL_BUILD_TESTS)
    include(CTest)
    add_subdirectory(test)
endif()

# Add documentation subdirectory if enabled
if(CCSL_BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        # Configure Doxygen
        configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            @ONLY
        )
        # Add doc target
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )
    else()
        message(STATUS "Doxygen not found, documentation will not be built")
    endif()
endif()

# Install targets
install(TARGETS ccsl code-checker
    EXPORT ccsl-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install headers
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

# Export targets
install(EXPORT ccsl-targets
    FILE ccsl-targets.cmake
    NAMESPACE CCSL::
    DESTINATION lib/cmake/ccsl
)

# Create and install package configuration file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ccsl-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ccsl-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ccsl-config.cmake"
    @ONLY
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/ccsl-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/ccsl-config-version.cmake"
    DESTINATION lib/cmake/ccsl
)