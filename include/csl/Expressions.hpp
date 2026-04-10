/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_EXPRESSIONS_HPP
#define CSL_EXPRESSIONS_HPP

#include <csl/Export.hpp>
#include <csl/ValueType.hpp>

#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace csl
{

class Node;
using NodePtr      = std::shared_ptr<Node>;
using ConstNodePtr = std::shared_ptr<const Node>;

class CSL_API ExpressionBase
{
public:
	ExpressionBase(ValueType typeId);

	ValueType valueType() const;

	NodePtr node();

	ConstNodePtr node() const;

	virtual void setNode(NodePtr node);

private:

	std::weak_ptr<Node> mNode;

	ValueType mValueType{};
};


/// Class defining a constant scalar value expression
template<typename Scalar>
class CSL_API ConstantExpression : public ExpressionBase
{
public:

	ConstantExpression(Scalar value) requires std::same_as<Scalar, int>
		: ExpressionBase(ValueType::INT)
		, mValue(value)
	{
	}

	ConstantExpression(Scalar value) requires std::same_as<Scalar, float>
		: ExpressionBase(ValueType::FLOAT)
		, mValue(value)
	{
	}

	ConstantExpression(Scalar value) requires std::same_as<Scalar, bool>
		: ExpressionBase(ValueType::BOOL)
		, mValue(value)
	{
	}

	Scalar value() const
	{ 
		return mValue;
	}

private:

	Scalar mValue{ 0 };
};


enum DefaultSemantics
{
	/// Set the position of a vertex in homogenous space. Every vertex shader must write out a parameter with this output.
	/*
	 * \Note: The position output value must be a 4-component float vector.
	 * \Note: The SV_POSITION attribute is only accessible in vertex shaders.
	 */
	SV_POSITION,

	/// Shader output that is used to override the z buffer value in the fragment shader.
	/*
	 * \Note: The depth output value must be a single float.
	 * \Note: the SV_DEPTH attribute is only accessible in fragment shaders.
	 */
	SV_DEPTH,
};


/// Class defining a shader input attrribute
class CSL_API InputAttributeExpression : public ExpressionBase
{
public:

	InputAttributeExpression(const ValueType& type, uint32_t location, std::string_view name);

	const std::string& name() const;

	uint32_t location() const;

	void setNode(NodePtr node) override;

private:

	uint32_t mLocation;

	std::string mName;
};


struct AttributeBinding
{
	std::string name;
	uint32_t location;
};

using AttributeBindingInfo = std::variant<AttributeBinding, DefaultSemantics>;

/// Class defining a shader output attribute
class CSL_API OutputAttributeExpression : public ExpressionBase
{
public:

	OutputAttributeExpression(ValueType valueType, uint32_t location, std::string_view name);

	OutputAttributeExpression(ValueType valueType, DefaultSemantics attribute);

	const AttributeBindingInfo& BindingInfo() const;

private:

	AttributeBindingInfo mBindingInfo;
};


class UniformBufferInfo
{
public:

	UniformBufferInfo(uint32_t location, std::string_view name);

	const std::string& name() const;

	uint32_t location() const;

private:

	uint32_t mLocation;

	std::string mName;
};


class CSL_API UniformExpression : public ExpressionBase
{
public:

	UniformExpression(const ValueType& type, std::string_view name, std::shared_ptr<UniformBufferInfo> buffer);

	void setNode(NodePtr node) override;

	std::shared_ptr<const UniformBufferInfo> bufferInfo() const;

	const std::string& name() const;

private:

	std::string mName;

	std::shared_ptr< UniformBufferInfo> mBufferInfo;
};


class CSL_API SamplerExpression : public ExpressionBase
{
public:

	SamplerExpression(uint32_t location, std::string_view name);

	const std::string& name() const;

	uint32_t location() const;

private:

	std::string mName;
	uint32_t mLocation;
};


enum class Operator
{
	// '=' operator
	ASSIGNMENT,
	// '*' operator
	MULTIPLY,
	// '/' operator
	DIVIDE,
	// '+' operator
	ADD,
	// '-' operator
	SUBTRACT,
	// '>' operator
	GREATER,
	// '<' operator
	LESS,
	// '==' operator
	EQUAL,
	// '>=' operator
	GREATER_OR_EQUAL,
	// '<=' operator
	LESS_OR_EQUAL,
	// '!=' operator
	NOT_EQUAL
};


/// Class defining an arithmetic operator expression
class CSL_API OperatorExpression : public ExpressionBase
{
public:

	OperatorExpression(ValueType outputType, Operator op);

	Operator getOperator() const;

private:

	Operator mOperator;
};


/// Class defining a cast operator expression
class CSL_API CastExpression : public ExpressionBase
{
public:

	CastExpression(ValueType outputType);
};


enum class NativeFunction
{
// --------------------------------------------------------------------------------------------------------------------
// Vector functions
// --------------------------------------------------------------------------------------------------------------------

	// Normalize a vector
	NORMALIZE, 
	// Calculate the dot product of two vectors
	DOT, 
	// Calculate the cross product of two vectors
	CROSS,
	// Calculate the length of a vector
	LENGTH,
	// Calculate the distance between two vectors
	DISTANCE,

// --------------------------------------------------------------------------------------------------------------------
// Texture functions
// --------------------------------------------------------------------------------------------------------------------

	// Read from a sampler
	SAMPLE,
	// Size of a texture
	TEXTURE_SIZE,
	// Discard the pixel
	DISCARD,

// --------------------------------------------------------------------------------------------------------------------
// Math functions
// --------------------------------------------------------------------------------------------------------------------

	// Linearly interpolate between two values
	MIX,
	// Return the lesser of two values
	MIN,
	// Return the greater of the two values
	MAX,
	// Return the absolute value of the value
	ABS,
	// Return the sign of the parameter
	SIGN,

// --------------------------------------------------------------------------------------------------------------------
// Trigonometric functions
// --------------------------------------------------------------------------------------------------------------------

	// Return the square root of the parameter
	SQRT,
	// Return the inverse of the square root of the parameter
	INVERSESQRT,
	// Return the value of the first parameter raised to the power of the second
	POW,
	// Return the sine of the parameter
	SIN,
	// Return the cosine of the parameter
	COS,
	// Return the tangent of the parameter
	TAN,
	// Return the arcsine of the parameter
	ASIN,
	// Return the arccosine of the parameter
	ACOS,
	// Return the arc-tangent of the parameters
	ATAN,

};


/// Class defining a native function call in the shader graph
class CSL_API NativeFunctionExpression : public ExpressionBase
{
public:

	NativeFunctionExpression(ValueType valueType, NativeFunction function);

	NativeFunction Function() const;

private:

	NativeFunction mFunction;
};


/// Class defining a native constructor call in the shader graph
class CSL_API ConstructorExpression : public ExpressionBase
{
public:

	ConstructorExpression(ValueType valueType);
};

enum class Swizzle
{
	X,
	Y,
	Z,
	W,
	XY,
	XYZ,
};

/// Class defining a swizzle operation in the shader graph
class CSL_API SwizzleExpression : public ExpressionBase
{
public:

	SwizzleExpression(ValueType outputType, Swizzle swizzle);

	Swizzle swizzle() const;

private:

	Swizzle mSwizzle;
};


class CSL_API BeginScopeExpression : public ExpressionBase
{
public:
	BeginScopeExpression();
};

class CSL_API EndScopeExpression : public ExpressionBase
{
public:
	EndScopeExpression();
};

class CSL_API IfExpression : public ExpressionBase
{
public:
	IfExpression();
};

class CSL_API ElseIfExpression : public ExpressionBase
{
public:
	ElseIfExpression();
};

class CSL_API ElseExpression : public ExpressionBase
{
public:
	ElseExpression();
};

using Expression = std::variant<ConstantExpression<float>, 
	                            ConstantExpression<int>, 
	                            ConstantExpression<bool>,
	                            InputAttributeExpression,
	                            OutputAttributeExpression,
	                            UniformExpression,
	                            SamplerExpression,
	                            OperatorExpression,
	                            CastExpression,
	                            NativeFunctionExpression,
	                            ConstructorExpression,
	                            SwizzleExpression, 
								IfExpression, 
								ElseIfExpression,
								ElseExpression>;

} // namespace csl

#endif // !CSL_EXPRESSIONS_HPP
