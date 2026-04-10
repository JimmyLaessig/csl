#ifndef CSL_EXAMPLES_DIFFUSETEXTUREFRAGMENTSHADER_HPP
#define CSL_EXAMPLES_DIFFUSETEXTUREFRAGMENTSHADER_HPP

#include <csl/csl.hpp>
#include "SimpleVertexShader.hpp"

namespace csl::Examples
{

using FragmentInput = csl::Examples::VertexOutput;


struct FragmentOutput
{
	csl::Vec4F color;
};


struct LightData
{
	csl::Vec4F lightColor;
	csl::Vec4F lightDirection;
};


inline void DefineType(FragmentOutput& output, csl::Attributes& attrs)
{
	attrs.RegisterAttribute(output.color, 0, "Color");
}


inline void DefineType(LightData& lightData, csl::UniformBufferBase& uniformBuffer)
{
	uniformBuffer.RegisterMember(lightData.lightColor, "lightColor");
	uniformBuffer.RegisterMember(lightData.lightDirection, "lightDirection");
}


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

		csl::Float b = csl::length(worldNormal);

		csl::Float c = 0;
		csl::If(b > 0.f, [&]
		{
		})
		.ElseIf(b > c, [&]
		{
		})
		.Else([&]
		{
		});
		

	
		return output;
	}
};

} // namespace csl::Examples

#endif // !CSL_EXAMPLES_DIFFUSETEXTUREFRAGMENTSHADER_HPP