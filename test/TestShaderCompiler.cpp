#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <csl/csl.hpp>

#include <DiffuseTextureShader.hpp>

TEST_CASE("Compile examples", "[csl]")
{
    SECTION("Compile DiffuseTextureShader")
    {
        DiffuseTextureShader::VertexShader vertexShader;

        csl::ShaderGraph shaderGraph(vertexShader);

        csl::CompilerGLSL compiler;
        auto result = compiler.Compile(shaderGraph, csl::ShaderStage::VERTEX);

        REQUIRE(result);
    }

    SECTION("Compile OpacityMaskShader")
    {
        DiffuseTextureShader::VertexShader vertexShader;

        csl::ShaderGraph shaderGraph(vertexShader);

        csl::CompilerGLSL compiler;
        auto result = compiler.Compile(shaderGraph, csl::ShaderStage::VERTEX);

        REQUIRE(result);
    }
}
