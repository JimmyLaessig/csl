cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME csl)

# The global version of the Reef Engine project
set(CSL_VERSION_MAJOR 0)
set(CSL_VERSION_MINOR 1)
set(CSL_VERSION_PATCH 0)
set(CSL_PROJECT_VERSION ${CSL_VERSION_MAJOR}.${CSL_VERSION_MINOR}.${CSL_VERSION_PATCH})

project(${PROJECT_NAME} VERSION ${CSL_PROJECT_VERSION})

set(TARGET_NAME csl)
set(ALIAS_TARGET_NAME csl::csl)

string(TOUPPER "${TARGET_NAME}" TARGET_NAME_UPPERCASE)

include(cmake/ThirdParty.cmake)

#######################################################################################################################
# Build the library as static or shared
#######################################################################################################################

option(CSL_BUILD_SHARED_LIBRARY "Build and link ${TARGET_NAME} as a shared library" OFF)

if (CSL_BUILD_SHARED_LIBRARY)
    set(TARGET_TYPE SHARED)
else()
    set(TARGET_TYPE STATIC)
endif()

#######################################################################################################################
# Create the CSL library
#######################################################################################################################

add_library(${TARGET_NAME} ${TARGET_TYPE})
add_library(${ALIAS_TARGET_NAME} ALIAS ${TARGET_NAME})

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(GENERATED_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/include)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

#######################################################################################################################
# Generate the export header
#######################################################################################################################

include(GenerateExportHeader)

set(EXPORT_HEADER ${CMAKE_CURRENT_BINARY_DIR}/generated/include/${TARGET_NAME}/Export.hpp)

# The generate_export_header function is used to generate a file containing suitable preprocessor for the EXPORT macros
# to be used in library classes. The header is automatically populated with the proper preprocessor defines based on
# the platform and build type (shared, static). (https://cmake.org/cmake/help/latest/module/GenerateExportHeader.html)
generate_export_header(${TARGET_NAME}
    EXPORT_FILE_NAME ${EXPORT_HEADER}
    EXPORT_MACRO_NAME ${TARGET_NAME_UPPERCASE}_API)

if (${TARGET_TYPE} STREQUAL "STATIC")
    target_compile_definitions(${TARGET_NAME} PUBLIC "${TARGET_NAME_UPPERCASE}_STATIC_DEFINE")
endif()

#######################################################################################################################
# Add the source files to the library
#######################################################################################################################

set(HEADERS
    ${EXPORT_HEADER}
    ${INCLUDE_DIR}/csl/Attribute.hpp
    ${INCLUDE_DIR}/csl/Compiler.hpp
    ${INCLUDE_DIR}/csl/CompilerGLSL.hpp
    ${INCLUDE_DIR}/csl/CompilerSPV.hpp
    ${INCLUDE_DIR}/csl/ControlFlow.hpp
    ${INCLUDE_DIR}/csl/csl.hpp
    ${INCLUDE_DIR}/csl/Expressions.hpp
    ${INCLUDE_DIR}/csl/Functions.hpp
    ${INCLUDE_DIR}/csl/Matrix.hpp
    ${INCLUDE_DIR}/csl/Node.hpp
    ${INCLUDE_DIR}/csl/Sampler.hpp
    ${INCLUDE_DIR}/csl/ShaderGraph.hpp
    ${INCLUDE_DIR}/csl/UniformBuffer.hpp
    ${INCLUDE_DIR}/csl/Value.hpp
    ${INCLUDE_DIR}/csl/ValueType.hpp
    ${INCLUDE_DIR}/csl/Vector.hpp
    ${INCLUDE_DIR}/csl/Visitor.hpp)

target_sources(${TARGET_NAME}
    PUBLIC
    FILE_SET  public_headers
    TYPE      HEADERS
    BASE_DIRS ${INCLUDE_DIR} ${GENERATED_INCLUDE_DIR}
    FILES     ${HEADERS})

set(SOURCES
    ${SOURCE_DIR}/CompilerGLSL.cpp
    ${SOURCE_DIR}/CompilerSPV.cpp
    ${SOURCE_DIR}/Expressions.cpp
    ${SOURCE_DIR}/Node.cpp
    ${SOURCE_DIR}/ShaderGraph.cpp
    ${SOURCE_DIR}/Value.cpp)

target_sources(${TARGET_NAME}
    PRIVATE
    ${SOURCES})

target_link_libraries(${TARGET_NAME}
    PRIVATE
    glslang
    glslang-default-resource-limits)

#######################################################################################################################
# Set target properties
#######################################################################################################################

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_23)

#######################################################################################################################
# Organize files in the IDE
#######################################################################################################################

source_group("include/csl" FILES ${HEADERS})
source_group("src/" FILES ${SOURCES})

# The visual studio compiler creates a .pdb file containing the debug information of the library. Setting the following
# property ensures the correct naming and output directory of the .pdb file.
if(MSVC)
    set_target_properties(${TARGET_NAME}
                          PROPERTIES
                          COMPILE_PDB_NAME              ${TARGET_NAME}
                          COMPILE_PDB_OUTPUT_DIR        "${CMAKE_CURRENT_BINARY_DIR}"
                          VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")
endif()

add_subdirectory(examples)
add_subdirectory(test)
