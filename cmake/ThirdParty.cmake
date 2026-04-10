file(
   DOWNLOAD
   https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.1/get_cpm.cmake
   ${CMAKE_BINARY_DIR}/cmake/get_cpm.cmake
   EXPECTED_HASH SHA256=799143bb9c1fbd14dc9a513eaff03ee1a709c6ee16f32456b50443d66dd7affa
)

include(${CMAKE_BINARY_DIR}/cmake/get_cpm.cmake)

set(SKIP_SPIRV_TOOLS_INSTALL ON CACHE BOOL "" FORCE)
set(GLSLANG_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

CPMAddPackage(
  NAME glslang
  GITHUB_REPOSITORY KhronosGroup/glslang
  GIT_TAG vulkan-sdk-1.4.309.0
  PATCH_COMMAND python ./update_glslang_sources.py
)

CPMAddPackage(
    NAME catch2
    GIT_REPOSITORY catchorg/Catch2
    GIT_TAG v3.4.0
)
