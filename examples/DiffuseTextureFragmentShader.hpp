#ifndef CSL_EXAMPLES_DIFFUSETEXTUREFRAGMENTSHADER_HPP
#define CSL_EXAMPLES_DIFFUSETEXTUREFRAGMENTSHADER_HPP

#include <csl/csl.hpp>

#include "SimpleVertexShader.hpp"

namespace csl::Examples
{

using FragmentInput = csl::Examples::VertexOutput;


struct FragmentOutput
{
	csl::Vec4F color{ csl::RegisterAttribute<csl::Vec4F>(0, "Color") };
};


struct LightData
{
	csl::Vec4F lightColor    { csl::RegisterUniform<csl::Vec4F>("lightColor") };
	csl::Vec4F lightDirection{ csl::RegisterUniform<csl::Vec4F>("lightDirection") };
};


struct DiffuseTextureFragmentShader
{
	csl::UniformBuffer<LightData> lightData{ 1, "LightData" };
	csl::Sampler2D colorTexture{ 2, "ColorTexture" };

	FragmentOutput main(const FragmentInput& input)
	{
		FragmentOutput output;
		auto worldNormal = csl::normalize(input.worldNormal);
		auto lightDir    = lightData->lightDirection.xyz();
		auto n_dot_l     = csl::dot(lightDir, worldNormal);
		auto color       = colorTexture.sample(input.texcoord0).xyz();
		output.color     = csl::Vec4F(color * lightData->lightColor.xyz() * n_dot_l, 1.f);

		return output;
	}
};

} // namespace csl::Examples

#endif // !CSL_EXAMPLES_DIFFUSETEXTUREFRAGMENTSHADER_HPP