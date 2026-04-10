

include(CTest)

set(TARGET_NAME UnitTests)
set(ALIAS_TARGET_NAME csl::UnitTests)

add_executable(${TARGET_NAME})
add_executable(${ALIAS_TARGET_NAME} ALIAS ${TARGET_NAME})

target_sources(${TARGET_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/TestShaderCompiler.cpp
)

target_link_libraries(${TARGET_NAME}
    PRIVATE
    csl::csl
    csl::Examples
    Catch2::Catch2WithMain
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
