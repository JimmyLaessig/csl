/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_ATTRIBUTE_HPP
#define CSL_ATTRIBUTE_HPP

#include <csl/Value.hpp>

#include <string_view>

namespace csl
{

ExpressionPtr RegisterAttribute(ValueType type, uint32_t location, std::string_view name);

ExpressionPtr RegisterAttribute(ValueType type, DefaultSemantics semantic);

template<typename T>
ExpressionPtr RegisterAttribute(uint32_t location, std::string_view name)
{
	return RegisterAttribute(T::ValueType, location, name);
}

template<typename T>
ExpressionPtr RegisterAttribute(DefaultSemantics semantic)
{
	return RegisterAttribute(T::ValueType, semantic);
}


class CSL_API Attributes
{
public:

	Attributes(bool input)
		: mInput(input)
	{
	}

	static inline void beginScope(Attributes& attributes)
	{
		sCurrent = &attributes;
	}

	static inline void endScope()
	{
		sCurrent = nullptr;
	}

	static inline Attributes* current()
	{
		return sCurrent;
	}

	bool isInput() const
	{ 
		return mInput;
	}

private:

	bool mInput;

	static inline Attributes* sCurrent{ nullptr };
};

} // namespace csl 

#endif // !CSL_ATTRIBUTE_HPP
