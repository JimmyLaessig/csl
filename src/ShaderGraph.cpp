/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#include <csl/ShaderGraph.hpp>

#include <csl/Expressions.hpp>
#include <csl/Node.hpp>

#include <cassert>
#include <ranges>
#include <set>


using namespace csl;


std::vector<const InputAttributeExpression*>
ShaderGraph::inputs() const
{
	std::vector<const InputAttributeExpression*> result;
	std::set<const InputAttributeExpression*> visited;
	for (auto node : mNodes)
	{
		if (auto attr = std::get_if<InputAttributeExpression>(&node->expression()))
		{
			if (visited.insert(attr).second)
			{
				result.push_back(attr);
			}
		}
	}

	return result;
}


std::vector<const UniformExpression*>
ShaderGraph::uniforms() const
{
	std::vector<const UniformExpression*> result;
	std::set<const UniformExpression*> visited;
	for (auto node : mNodes)
	{
		if (auto uniform = std::get_if<UniformExpression>(&node->expression()))
		{
			if (visited.insert(uniform).second)
			{
				result.push_back(uniform);
			}
		}
	}

	return result;
}


std::vector<const OutputAttributeExpression*>
ShaderGraph::outputs() const
{
	std::vector<const OutputAttributeExpression*> result;
	std::set<const OutputAttributeExpression*> visited;

	for (auto node : mNodes)
	{
		if (auto attr = std::get_if<OutputAttributeExpression>(&node->expression()))
		{
			if (visited.insert(attr).second)
			{
				result.push_back(attr);
			}
		}
	}

	return result;
}


std::vector<const Expression*>
ShaderGraph::expressions() const
{
	return mNodes
		| std::views::transform([](const auto& node) { return &node->expression(); })
		| std::ranges::to<std::vector<const Expression*>>();
}


void
ShaderGraph::addNode(std::shared_ptr<Node> node)
{
	mNodes.push_back(node);
}


static thread_local ShaderGraph* sCurrent = nullptr;

void
ShaderGraph::beginRecording()
{
	assert(!sCurrent && "Only one shader graph can be recorded at a time");
	sCurrent = this;
}


void
ShaderGraph::endRecording()
{
	assert(sCurrent == this && "Shader graph recording not start.");
	sCurrent = nullptr;
}


ShaderGraph*
ShaderGraph::current()
{
	return sCurrent;
}
