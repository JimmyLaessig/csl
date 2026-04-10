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

Value::Value(std::shared_ptr<Node> source)
	: mNode(source)
{
}


std::shared_ptr<Node>
Value::node() const &
{
	return mNode;
}


std::shared_ptr<Node>
Value::node() &&
{
	// Mark the source of this expression to be inlined if this is an r-value reference
	mNode->setInlineIfPossible();
	return mNode;
}


void
Value::setNode(std::shared_ptr<Node> source)
{
	mNode = source;
}
