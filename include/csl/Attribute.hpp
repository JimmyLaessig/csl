/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_ATTRIBUTE_HPP
#define CSL_ATTRIBUTE_HPP

#include <csl/Expressions.hpp>
#include <csl/Value.hpp>

#include <string_view>

namespace csl
{

class CSL_API Attributes
{
public:

	Attributes(bool input)
		: mInput(input)
	{}

	template<typename T>
	void RegisterAttribute(T& value, uint32_t location, std::string_view name)
	{
		if (mInput)
		{
			value.node()->setExpression(InputAttributeExpression(value.node()->valueType(), location, name));
		}
		else
		{
			Node::create(OutputAttributeExpression(value.node()->valueType(), location, name), value.node());
		}
	}

	template<typename T>
	void RegisterAttribute(T& value, csl::DefaultSemantics semantic)
	{
		if (!mInput)
		{
			Node::create(OutputAttributeExpression(value.node()->valueType(), semantic), value.node());
		}
		else
		{
			value.node()->setInlineIfPossible();
		}
	}

private:

	bool mInput;
};

} // namespace csl 

#endif // !CSL_ATTRIBUTE_HPP
