/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#include <csl/ShaderGraph.hpp>

#include <csl/Expressions.hpp>


#include <cassert>
#include <ranges>
#include <set>


using namespace csl;


std::vector<const InputAttributeExpression*>
ShaderGraph::inputs() const
{
	return mExpressions
		| std::views::transform([](auto expr) { return expr->cast<InputAttributeExpression>(); })
		| std::views::filter([](auto expr)    { return expr != nullptr; })
		| std::ranges::to<std::vector<const InputAttributeExpression*>>();
}


std::vector<const UniformExpression*>
ShaderGraph::uniforms() const
{
	return mExpressions
		| std::views::transform([](auto expr) { return expr->cast<UniformExpression>(); })
		| std::views::filter([](auto expr)    { return expr != nullptr; })
		| std::ranges::to<std::vector<const UniformExpression*>>();
}


std::vector<const OutputAttributeExpression*>
ShaderGraph::outputs() const
{
	return mExpressions
		| std::views::transform([](auto expr) { return expr->cast<OutputAttributeExpression>(); })
		| std::views::filter([](auto expr)    { return expr != nullptr; })
		| std::ranges::to<std::vector<const OutputAttributeExpression*>>();
}


std::vector<const Expression*>
ShaderGraph::expressions() const
{
	return mExpressions
		| std::views::transform([](const auto& expr) { return expr.get(); })
		| std::ranges::to<std::vector<const Expression*>>();
}


void
ShaderGraph::addNode(std::shared_ptr<Expression> expr)
{
	mExpressions.push_back(expr);
}


static thread_local ShaderGraph* sCurrent = nullptr;

void
ShaderGraph::beginScope(ShaderGraph& shaderGraph)
{
	assert(!sCurrent && "Only one shader graph can be recorded at a time");
	sCurrent = &shaderGraph;
}


void
ShaderGraph::endScope()
{
	assert(sCurrent && "Shader graph recording not start.");
	sCurrent = nullptr;
}


ShaderGraph*
ShaderGraph::current()
{
	return sCurrent;
}
