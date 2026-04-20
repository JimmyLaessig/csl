///**
// * Copyright(c) 2026 Bernhard Rainer
// * SPDX-License-Identifier: MIT
// *
// * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
// * See the LICENSE file in the project root for full license information.
// */
//
//#include <csl/Node.hpp>
//
//#include <csl/ShaderGraph.hpp>
//#include <csl/Visitor.hpp>
//
//#include <ranges>
//
//using namespace csl;
//
//Node::Node(const Expression& expression, const std::vector<NodePtr>& inputs)
//	: mExpression(expression)
//	, mInputs(inputs)
//{
//}
//
//
//NodePtr
//Node::create(const Expression& expression)
//{
//	return create(expression, std::vector<NodePtr>{});
//}
//
//
//NodePtr
//Node::create(const Expression& expression, const std::vector<NodePtr>& inputs)
//{
//	auto node = std::make_shared<Node>(expression, inputs);
//
//	std::visit(Visitor{ [&](auto& ex) { ex.setNode(node); } }, node->mExpression);
//
//	if (auto shaderGraph = ShaderGraph::current())
//	{
//		shaderGraph->addNode(node);
//	}
//
//	return node;
//}
//
//
//ValueType 
//Node::valueType() const
//{
//	return std::visit(Visitor{ [](const auto& ex) { return ex.valueType(); } }, mExpression);
//}
//
//
//const Expression& 
//Node::expression() const
//{
//	return mExpression;
//}
//
//
//void 
//Node::setExpression(const Expression& expression)
//{
//	mExpression = expression;
//	auto self = shared_from_this();
//	std::visit(Visitor{ [self](auto& ex) { ex.setNode(self); } }, mExpression);
//}
//
//
//std::vector<std::shared_ptr<const Node>> 
//Node::inputs() const
//{
//	return mInputs | std::ranges::to<std::vector<std::shared_ptr<const Node>>>();
//}
//
//
//bool 
//Node::inlineIfPossible() const
//{
//	return mInline;
//}
//
//
//void 
//Node::setInlineIfPossible()
//{
//	mInline = true;
//}
