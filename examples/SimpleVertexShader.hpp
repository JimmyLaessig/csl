#ifndef CSL_EXAMPLES_SIMPLEVERTEXSHADER_HPP
#define CSL_EXAMPLES_SIMPLEVERTEXSHADER_HPP

#include <csl/csl.hpp>

namespace csl::Examples
{

struct VertexInput
{
	csl::Vec3F position { csl::RegisterAttribute<csl::Vec3F>(0, "Position") };
	csl::Vec3F normal   { csl::RegisterAttribute<csl::Vec3F>(1, "Normal") };
	csl::Vec2F texcoord0{ csl::RegisterAttribute<csl::Vec2F>(2, "Texcoord0") };
};

struct VertexOutput
{
	csl::Vec4F position   { csl::RegisterAttribute<csl::Vec4F>(csl::DefaultSemantics::SV_POSITION) };
	csl::Vec3F worldNormal{ csl::RegisterAttribute<csl::Vec3F>(0, "WorldNormal")};
	csl::Vec2F texcoord0  { csl::RegisterAttribute<csl::Vec2F>(1, "Texcoord0")};
};

struct Matrices
{
	csl::Mat44F modelViewProjectionMatrix{ csl::RegisterUniform<csl::Mat44F>("modelViewProjectionMatrix")};
	csl::Mat33F normalMatrix             { csl::RegisterUniform<csl::Mat33F>("normalMatrix") };
};


struct SimpleVertexShader
{
	csl::UniformBuffer<Matrices> matrices{ 0, "Uniforms" };

	VertexOutput main(const VertexInput& input)
	{
		VertexOutput output;
		output.position    = matrices->modelViewProjectionMatrix * csl::Vec4F(input.position, 1.f);
		// Normalize is not correctly inlined
		output.worldNormal = csl::normalize(matrices->normalMatrix * (input.normal));
		output.texcoord0   = input.texcoord0;
		return output;
	}
};

} // namespace csl::Examples

#endif // !CSL_EXAMPLES_SIMPLEVERTEXSHADER_HPP
