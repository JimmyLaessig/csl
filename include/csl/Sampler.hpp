/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_SAMPLER_HPP
#define CSL_SAMPLER_HPP

#include <csl/Vector.hpp>

#include <csl/Node.hpp>

namespace csl
{

struct CSL_API Sampler2D : Value
{
	Sampler2D(uint32_t location, std::string_view name)
		: Value(nullptr)
		, mName(name)
		, mLocation(location)
	{
	}

	static constexpr ValueType toValueType() { return ValueType::SAMPLER2D; }

	template<VectorType<float, 2> Vec>
	Vec4F sample(Vec&& uv) const
	{
		lazyInit();
		return { Expression::create<NativeFunctionExpression>(ValueType::VEC4F, 
			                                                  NativeFunction::SAMPLE,
															  expression(),
															  std::forward<Vec>(uv).expression()) };
	}

	template<ScalarType<int> S>
	Vec2I size(S&& lod) const
	{
		lazyInit();
		return { Expression::create<NativeFunctionExpression>(ValueType::VEC3F, 
			                                                  NativeFunction::TEXTURE_SIZE,
			                                                  expression(), 
			                                                  std::forward<S>(lod).expression()) };
	}

	Sampler2D(const Sampler2D&) = delete;
	Sampler2D& operator=(const Sampler2D&) = delete;

	Sampler2D(Sampler2D&&) = delete;
	Sampler2D& operator=(Sampler2D&&) = delete;

private:

	void lazyInit() const
	{
		auto* self = const_cast<Sampler2D*>(this);
		if (!self->expression())
		{
			self->setExpression(Expression::create<SamplerExpression>(mLocation, mName));
		}
	}

	uint32_t mLocation;
	std::string mName;
};

} // namespace csl 

#endif // !CSL_SAMPLER_HPP
