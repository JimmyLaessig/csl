

set(TARGET_NAME Examples)
set(ALIAS_TARGET_NAME csl::Examples)

add_library(${TARGET_NAME} INTERFACE)
add_library(${ALIAS_TARGET_NAME} ALIAS ${TARGET_NAME})

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HEADERS
    ${INCLUDE_DIR}/DiffuseTextureFragmentShader.hpp
    ${INCLUDE_DIR}/OpacityMaskFragmentShader.hpp
    ${INCLUDE_DIR}/SimpleVertexShader.hpp)

target_sources(${TARGET_NAME}
    PUBLIC
    FILE_SET  public_headers
    TYPE      HEADERS
    BASE_DIRS ${INCLUDE_DIR}
    FILES     ${HEADERS})

target_link_libraries(${TARGET_NAME}
    INTERFACE
    csl::csl)
