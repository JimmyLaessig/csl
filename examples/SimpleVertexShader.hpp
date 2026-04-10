#ifndef CSL_EXAMPLES_SIMPLEVERTEXSHADER_HPP
#define CSL_EXAMPLES_SIMPLEVERTEXSHADER_HPP

#include <csl/csl.hpp>

namespace csl::Examples
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

struct Matrices
{
	csl::Mat44F modelViewProjectionMatrix;
	csl::Mat33F normalMatrix;
};


inline void DefineType(VertexInput& v, csl::Attributes& attrs)
{
	attrs.RegisterAttribute(v.position,  0, "Position");
	attrs.RegisterAttribute(v.normal,    1, "Normal");
	attrs.RegisterAttribute(v.texcoord0, 2, "Texcoord0");
}


inline void DefineType(VertexOutput& v, csl::Attributes& attrs)
{
	attrs.RegisterAttribute(v.position, csl::SV_POSITION);
	attrs.RegisterAttribute(v.worldNormal, 0, "Normal");
	attrs.RegisterAttribute(v.texcoord0,   1, "Texcoord0");
}


inline void DefineType(Matrices& matrices, csl::UniformBufferBase& uniformBuffer)
{
	uniformBuffer.RegisterMember(matrices.modelViewProjectionMatrix, "modelViewProjectionMatrix");
	uniformBuffer.RegisterMember(matrices.normalMatrix, "normalMatrix");
}


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
