/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#include <csl/Value.hpp>

#include <csl/Node.hpp>

using namespace csl;

Value::Value(ExpressionPtr source)
	: mExpression(source)
{
}


ExpressionPtr
Value::expression() const &
{
	return mExpression;
}


ExpressionPtr
Value::expression() &&
{
	// Mark the source of this expression to be inlined if this is an r-value reference
	mExpression->forceInlineResult();
	return mExpression;
}


void
Value::setExpression(ExpressionPtr source)
{
	mExpression = source;
}
