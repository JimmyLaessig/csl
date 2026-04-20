/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#include <csl/Expressions.hpp>

#include <csl/ShaderGraph.hpp>

using namespace csl;

Expression::Expression(ValueType typeId, const std::vector<ExpressionPtr>& inputs)
	: mValueType(typeId)
	, mInputs(inputs)
{
}


Expression::Expression(ValueType typeId)
	: mValueType(typeId)
{
}


ValueType
Expression::valueType() const
{
	return mValueType;
}


void
Expression::construct()
{
	if (auto shaderGraph = ShaderGraph::current())
	{
		auto pThis = shared_from_this();
		shaderGraph->addNode(pThis);
	}
}


std::vector<ConstExpressionPtr>
Expression::inputs() const
{
	std::vector<ConstExpressionPtr> result;
	for (auto input : mInputs)
	{
		result.push_back(input);
	}
	return result;
}


bool
Expression::inlineResult() const
{
	return mInline;
}


void
Expression::forceInlineResult()
{
	mInline = true;
}


InputAttributeExpression::InputAttributeExpression(const ValueType& type, uint32_t location, std::string_view name)
	: ExpressionBase<InputAttributeExpression>(type)
	, mLocation(location)
	, mName(name)
{
	forceInlineResult();
}


const std::string&
InputAttributeExpression::name() const
{ 
	return mName;
}


uint32_t
InputAttributeExpression::location() const 
{ 
	return mLocation;
}


OutputAttributeExpression::OutputAttributeExpression(ValueType valueType, uint32_t location, std::string_view name)
	: ExpressionBase<OutputAttributeExpression>(valueType)
	, mBindingInfo(AttributeBinding{ std::string(name), location })
{
}


OutputAttributeExpression::OutputAttributeExpression(ValueType valueType, DefaultSemantics attribute)
	: ExpressionBase<OutputAttributeExpression>(valueType)
	, mBindingInfo(attribute)
{
}


const AttributeBindingInfo&
OutputAttributeExpression::BindingInfo() const
{ 
	return mBindingInfo;
}


UniformBufferExpression::UniformBufferExpression(uint32_t location, std::string_view name)
	: ExpressionBase<UniformBufferExpression>(ValueType::VOID)
	, mLocation(location)
	, mName(name)
{
}


const std::string&
UniformBufferExpression::name() const
{
	return mName;
}


uint32_t 
UniformBufferExpression::location() const
{
	return mLocation;
}


UniformExpression::UniformExpression(const ValueType& type, std::string_view name, std::shared_ptr<UniformBufferExpression> buffer)
	: ExpressionBase<UniformExpression>(type, buffer)
	, mName(name)
{
	forceInlineResult();
}


const std::string&
UniformExpression::name() const
{ 
	return mName;
}


SamplerExpression::SamplerExpression(uint32_t location, std::string_view name)
	: ExpressionBase<SamplerExpression>(ValueType::SAMPLER2D)
	, mName(name)
	, mLocation(location)
{
}


const std::string&
SamplerExpression::name() const
{
	return mName;
}


uint32_t
SamplerExpression::location() const
{ 
	return mLocation;
}


OperatorExpression::OperatorExpression(ValueType outputType, Operator op, ExpressionPtr arg0)
	: ExpressionBase<OperatorExpression>(outputType, { arg0 })
	, mOperator(op)
{
}


OperatorExpression::OperatorExpression(ValueType outputType, Operator op, ExpressionPtr arg0, ExpressionPtr arg1)
	: ExpressionBase<OperatorExpression>(outputType, { arg0, arg1 })
	, mOperator(op)
{
}


Operator 
OperatorExpression::getOperator() const
{
	return mOperator;
}


CastExpression::CastExpression(ValueType outputType, ExpressionPtr input)
	: ExpressionBase<CastExpression>(outputType, { input })
{
}


NativeFunction
	NativeFunctionExpression::Function() const
{ 
	return mFunction;
}


SwizzleExpression::SwizzleExpression(ValueType outputType, Swizzle swizzle, ExpressionPtr input)
	: ExpressionBase<SwizzleExpression>(outputType, { input })
	, mSwizzle(swizzle)
{
}


Swizzle
SwizzleExpression::swizzle() const
{ 
	return mSwizzle;
};


IfExpression::IfExpression(ExpressionPtr input)
	: ExpressionBase<IfExpression>(ValueType::VOID, input)
{
}


ElseIfExpression::ElseIfExpression(ExpressionPtr condition, std::shared_ptr<IfExpression> input)
	: ExpressionBase<ElseIfExpression>(ValueType::VOID, condition, input)
{
}


ElseIfExpression::ElseIfExpression(ExpressionPtr condition, std::shared_ptr<ElseIfExpression> input)
	: ExpressionBase<ElseIfExpression>(ValueType::VOID, condition, input)
{
}


ElseExpression::ElseExpression(std::shared_ptr<IfExpression> input)
	: ExpressionBase<ElseExpression>(ValueType::VOID, input)
{
}


ElseExpression::ElseExpression(std::shared_ptr<ElseIfExpression> input)
	: ExpressionBase<ElseExpression>(ValueType::VOID, input)
{
}


EndIfExpression::EndIfExpression(std::shared_ptr<IfExpression> input)
	: ExpressionBase<EndIfExpression>(ValueType::VOID, input)
{
}


EndIfExpression::EndIfExpression(std::shared_ptr<ElseIfExpression> input)
	: ExpressionBase<EndIfExpression>(ValueType::VOID, input)
{
}


EndIfExpression::EndIfExpression(std::shared_ptr<ElseExpression> input)
	: ExpressionBase<EndIfExpression>(ValueType::VOID, input)
{
}
