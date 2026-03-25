#ifndef CSL_SAMPLER_HPP
#define CSL_SAMPLER_HPP

#include <csl/Vector.hpp>
#include <csl/Attribute.hpp>

#include <csl/Node.hpp>

namespace csl
{

template<Semantic S, Location L>
struct CSL_API Sampler2D : Value
{
	Sampler2D()
		: Value(Node::create(SamplerExpression(static_cast<uint32_t>(L), S)))
	{
	}

	static constexpr ValueType toValueType() { return ValueType::SAMPLER2D; }

	template<typename Vec> 
		requires VectorType<Vec, float, 2>
	float4 sample(Vec&& uv) const
	{
		return float4(Node::create(NativeFunctionExpression(ValueType::FLOAT4, NativeFunction::SAMPLE),
			                       node(), 
			                       std::forward<Vec&&>(uv).node()));
	}

	template<typename S>
		requires ScalarType<S, int>
	int2 size(S&& lod) const
	{
		return { Node::create(NativeFunctionExpression(ValueType::FLOAT3, NativeFunction::TEXTURE_SIZE),
			                  node(), 
			                  std::forward<S&&>(lod).node()) };
	}

	Sampler2D(const Sampler2D&) = delete;
	Sampler2D& operator=(const Sampler2D&) = delete;

	Sampler2D(Sampler2D&&) = delete;
	Sampler2D& operator=(Sampler2D&&) = delete;
};

} // namespace ShaderLanguage 

#endif // !CSL_SAMPLER_HPP
