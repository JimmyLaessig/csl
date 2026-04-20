/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_UNIFORMBUFFER_HPP
#define CSL_UNIFORMBUFFER_HPP

#include <csl/Expressions.hpp>
#include <csl/ShaderGraph.hpp>
#include <csl/Node.hpp>

#include <optional>

namespace csl
{

class CSL_API UniformBufferBase
{
public:

	virtual ~UniformBufferBase() = default;

	UniformBufferBase(uint32_t location, std::string_view name);

	static void beginScope(UniformBufferBase& uniformBuffer);

	static void endScope();

	std::shared_ptr<UniformBufferExpression> expression() const;

private:

	std::shared_ptr<UniformBufferExpression> mExpression;
};


template<typename T>
class CSL_API UniformBuffer : public UniformBufferBase
{
public:

	using UniformBufferBase::UniformBufferBase;

	const T* operator->() const
	{
		lazyInit();
		return &mValue.value();
	}

	const T& operator*() const
	{
		lazyInit();
		return mValue.value();
	}

	operator T() const
	{
		lazyInit();
		return mValue.value();
	}

private:

	void lazyInit() const
	{
		if (mValue)
		{
			return;
		}
		auto self = const_cast<UniformBuffer<T>*>(this);

		beginScope(*self);
		mValue.emplace();
		endScope();
	}

	mutable std::optional<T> mValue;

};

ExpressionPtr
RegisterUniform(ValueType type, std::string_view name);

template<typename T>
ExpressionPtr RegisterUniform(std::string_view name)
{
	return RegisterUniform(T::ValueType, name);
}

} // namespace csl 

#endif // !CSL_UNIFORMBUFFER_HPP
