/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_VALUE_HPP
#define CSL_VALUE_HPP

#include <csl/Export.hpp>

#include <csl/ValueType.hpp>

#include <csl/Expressions.hpp>

#include <memory>

namespace csl
{

/// Base struct for all values of shader graph expressions
/**
 * Classes that derive from this type act as stack-allocated wrappers arround a shader graph expression and define the
 * output type of said operation. Classes of this type are widely used as inputs to functions that build up new shader
 * graph expressions. For example, the ´float32´ struct is a proxy for a 32-bit floating-point value that is created by
 * a shader graph expression. The object itself does not hold the actual value but stores a pointer to the shader graph
 * node from which it was calculated. The user is only required to interact with the stack-allocated Value objects and
 * function calls to build the shader graph. The creation and storage of the actual ShaderGraph is abstracted away via
 * this level of indirection.
 */
struct CSL_API Value
{
public:

	Value(ExpressionPtr source);

	// Get the wrapped ShaderGraph expression
	ExpressionPtr expression() const &;

	// Get the wrapped ShaderGraph expression
	ExpressionPtr expression() &&;

	void setExpression(ExpressionPtr source);

private:

	ExpressionPtr mExpression;
};

} // namespace csl 

#endif // !CSL_VALUE_HPP
