/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#include <csl/Expressions.hpp>

#include <csl/Node.hpp>

using namespace csl;

ExpressionBase::ExpressionBase(ValueType type)
	: mValueType(type)
{
}


ValueType
ExpressionBase::valueType() const
{
	return mValueType;
}


NodePtr
ExpressionBase::node()
{
	return mNode.lock();
}


ConstNodePtr
ExpressionBase::node() const
{
	return mNode.lock();
}


void
ExpressionBase::setNode(NodePtr node)
{
	mNode = node;
}


InputAttributeExpression::InputAttributeExpression(const ValueType& type, uint32_t location, std::string_view name)
	: ExpressionBase(type)
	, mLocation(location)
	, mName(name)
{
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

void
InputAttributeExpression::setNode(NodePtr node)
{
	ExpressionBase::setNode(node);
	node->setInlineIfPossible();
}


OutputAttributeExpression::OutputAttributeExpression(ValueType valueType, uint32_t location, std::string_view name)
	: ExpressionBase(valueType)
	, mBindingInfo(AttributeBinding{ std::string(name), location })
{
}


OutputAttributeExpression::OutputAttributeExpression(ValueType valueType, DefaultSemantics attribute)
	: ExpressionBase(valueType)
	, mBindingInfo(attribute)
{
}


const AttributeBindingInfo&
OutputAttributeExpression::BindingInfo() const
{ 
	return mBindingInfo;
}


UniformBufferInfo::UniformBufferInfo(uint32_t location, std::string_view name)
	: mLocation(location)
	, mName(name)
{
}


const std::string&
UniformBufferInfo::name() const
{
	return mName;
}


uint32_t 
UniformBufferInfo::location() const
{
	return mLocation;
}


UniformExpression::UniformExpression(const ValueType& type, std::string_view name, std::shared_ptr<UniformBufferInfo> buffer)
	: ExpressionBase(type)
	, mName(name)
	, mBufferInfo(buffer)
{
}


void
UniformExpression::setNode(NodePtr node)
{
	ExpressionBase::setNode(node);
	node->setInlineIfPossible();
}


std::shared_ptr<const UniformBufferInfo>
UniformExpression::bufferInfo() const
{
	return mBufferInfo;
}


const std::string&
UniformExpression::name() const
{ 
	return mName;
}


SamplerExpression::SamplerExpression(uint32_t location, std::string_view name)
	: ExpressionBase(ValueType::SAMPLER2D)
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


OperatorExpression::OperatorExpression(ValueType outputType, Operator op)
	: ExpressionBase(outputType)
	, mOperator(op)
{
}


Operator 
OperatorExpression::getOperator() const
{
	return mOperator;
}


CastExpression::CastExpression(ValueType outputType)
	: ExpressionBase(outputType)
{
}


NativeFunctionExpression::NativeFunctionExpression(ValueType valueType, NativeFunction function)
	: ExpressionBase(valueType)
	, mFunction(function)
{
}


NativeFunction
	NativeFunctionExpression::Function() const
{ 
	return mFunction;
}


ConstructorExpression::ConstructorExpression(ValueType valueType)
	: ExpressionBase(valueType)
{
}


SwizzleExpression::SwizzleExpression(ValueType outputType, Swizzle swizzle)
		: ExpressionBase(outputType)
		, mSwizzle(swizzle)
{
}


Swizzle
SwizzleExpression::swizzle() const
{ 
	return mSwizzle;
};


BeginScopeExpression::BeginScopeExpression()
	: ExpressionBase(ValueType::VOID)
{
}


EndScopeExpression::EndScopeExpression()
	: ExpressionBase(ValueType::VOID)
{
}


IfExpression::IfExpression()
	: ExpressionBase(ValueType::VOID)
{
}


ElseIfExpression::ElseIfExpression()
	: ExpressionBase(ValueType::VOID)
{
}


ElseExpression::ElseExpression()
	: ExpressionBase(ValueType::VOID)
{
}
