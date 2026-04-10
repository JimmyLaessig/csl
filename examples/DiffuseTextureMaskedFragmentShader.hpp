#ifndef CSL_EXAMPLES_DIFFUSETEXTUREMASKEDFRAGMENTSHADER_HPP
#define CSL_EXAMPLES_DIFFUSETEXTUREMASKEDFRAGMENTSHADER_HPP

#include <csl/csl.hpp>

namespace DiffuseTextureMaskedFragmentShader
{

	struct VertexInput
	{
		csl::Vec3F position;
		csl::Vec3F normal;
		csl::Vec2F texcoord0;
	};

	struct VertexOutput
	{
		csl::Vec4F position;
		csl::Vec3F worldNormal;
		csl::Vec2F texcoord0;
	};


	using FragmentInput = VertexOutput;


	struct FragmentOutput
	{
		csl::Vec4F color;
	};

	struct Uniforms
	{
		csl::Mat44F modelViewProjectionMatrix;
		csl::Mat33F normalMatrix;
		csl::Vec3F lightColor;
		csl::Vec3F lightDirection;
	};


	inline void DefineType(VertexInput& v, csl::Attributes& attrs)
	{
		attrs.RegisterAttribute<"Position", 0>(v.position);
		attrs.RegisterAttribute<"Normal", 1>(v.normal);
		attrs.RegisterAttribute<"Texcoord0", 2>(v.texcoord0);
	}


	inline void DefineType(VertexOutput& v, csl::Attributes& attrs)
	{
		attrs.RegisterAttribute<csl::SV_POSITION>(v.position);
		attrs.RegisterAttribute<"Normal", 0>(v.worldNormal);
		attrs.RegisterAttribute<"Texcoord0", 1>(v.texcoord0);
	}


	inline void DefineType(FragmentOutput& output, csl::Attributes& attrs)
	{
		attrs.RegisterAttribute<"Color", 0>(output.color);
	}


	inline void DefineType(Uniforms& uniforms, csl::UniformBufferBase& uniformBuffer)
	{
		uniformBuffer.RegisterMember<"modelViewProjectionMatrix">(uniforms.modelViewProjectionMatrix);
		uniformBuffer.RegisterMember<"normalMatrix">(uniforms.normalMatrix);
		uniformBuffer.RegisterMember<"lightColor">(uniforms.lightColor);
		uniformBuffer.RegisterMember<"lightDirection">(uniforms.lightDirection);
	}


	struct VertexShader
	{
		csl::UniformBuffer < Uniforms, "Uniforms", csl::Location{ 0 } > uniforms;

		VertexOutput main(const VertexInput& input)
		{
			VertexOutput output;
			output.position = uniforms->modelViewProjectionMatrix * csl::Vec4F(input.position, 1.f);
			// Normalize is not correctly inlined
			output.worldNormal = csl::normalize(uniforms->normalMatrix * (input.normal));
			output.texcoord0 = input.texcoord0;
			return output;
		}
	};


	struct FragmentShader
	{
		csl::UniformBuffer < Uniforms, "Uniforms", csl::Location{ 0 } > uniforms;
		csl::Sampler2D < "colorTexture", csl::Location{ 1 } > colorTexture;

		FragmentOutput main(const FragmentInput& input)
		{
			//csl::If(csl::Bool(true), [&]
			//{
			//})
			//.ElseIf(csl::Bool(false), [&]
			//{
			//})
			//.Else([&]
			//{
			//});

			FragmentOutput output;
			auto worldNormal = csl::normalize(input.worldNormal);
			auto lightDir = uniforms->lightDirection;
			auto n_dot_l = csl::dot(lightDir, worldNormal);
			auto color = colorTexture.sample(input.texcoord0).xyz();
			output.color = csl::Vec4F(color * uniforms->lightColor * n_dot_l, 1.f);
			return output;
		}
	};

} // namespace DiffuseTextureWithLightingShader

#endif // !CSL_EXAMPLES_DIFFUSETEXTUREWITHLIGHTINGSHADER_HPP
//			{
//			})
//			.ElseIf(csl::Bool(false), [&]
//				{
//				})
//			.Else([&]
//				{
//				});
//
//
//		FragmentOutput output;
//		auto worldNormal = csl::normalize(*input.worldNormal);
//		auto lightDir = uniforms->lightDirection;
//		auto n_dot_l = csl::dot(lightDir, worldNormal);
//		auto color = colorTexture.sample(*input.texcoord0).xyz();
//		output.color = csl::Vec4F(color * uniforms->lightColor * n_dot_l, 1.f);
//		return output;
//	}
//};
//
//} // namespace OpacityMaskShader
//
//#endif // !CSL_EXAMPLES_DIFFUSETEXTUREMASKEDFRAGMENTSHADER_HPP