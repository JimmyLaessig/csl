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

class Expression;
using ExpressionPtr      = std::shared_ptr<Expression>;
using ConstExpressionPtr = std::shared_ptr<const Expression>;

class CSL_API Expression : public std::enable_shared_from_this<Expression>
{
public:

	template<typename T, typename ...Args>
	static std::shared_ptr<T> create(Args&&... args)
	{
		auto expression = std::make_shared<T>(std::forward<Args>(args)...);
		expression->construct();

		return expression;
	}

	template<typename ...Expressions>
	Expression(ValueType valueType, Expressions&&... inputs)
		: mValueType(valueType)
		, mInputs({ std::forward<Expressions>(inputs)... })
	{
	}

	Expression(ValueType valueType, const std::vector<ExpressionPtr>& inputs);

	Expression(ValueType valueType);

	// \brief Get the expression's value type
	ValueType valueType() const;

	/** \brief Get the list of inputs this expression
	 * \return Returns a vector of pointers to this expression's inputs
	 */
	std::vector<ConstExpressionPtr> inputs() const;

	// \brief Flag indicating if the expression's result should be inlined during compilation
	bool inlineResult() const;

	/** \brief Mark the expression to be inlined during compilation
	 * Inlining an expression notifies the compiler to skip an explicit variable declaration to store the expression's
	 * result. Instead, the compiler is should construct the expression in-place.
	 */
	void forceInlineResult();

	virtual size_t typeId() const = 0;

	template<typename T>
	T* cast()
	{
		if (typeId() == T::classTypeId())
		{
			return static_cast<T*>(this);
		}
		return nullptr;
	}

	template<typename T>
	const T* cast() const
	{
		if (typeId() == T::classTypeId())
		{
			return static_cast<const T*>(this);
		}
		return nullptr;
	}

protected:

	virtual void construct();

private:

	ValueType mValueType{};

	std::vector<ExpressionPtr> mInputs;

	bool mInline{ false };

};

template<typename T>
class CSL_API ExpressionBase : public Expression
{
public:
	using Expression::Expression;

	size_t typeId() const
	{
		return ExpressionBase<T>::classTypeId();
	}

	static size_t classTypeId()
	{
		static char sTypeId;
		return reinterpret_cast<size_t>(&sTypeId);
	}
};


/// Class defining a constant scalar value expression
template<typename Scalar>
class CSL_API ConstantExpression : public ExpressionBase<ConstantExpression<Scalar>>
{
public:

	ConstantExpression(Scalar value) requires std::same_as<Scalar, int>
		: ExpressionBase<ConstantExpression<Scalar>>(ValueType::INT)
		, mValue(value)
	{
	}

	ConstantExpression(Scalar value) requires std::same_as<Scalar, float>
		: ExpressionBase<ConstantExpression<Scalar>>(ValueType::FLOAT)
		, mValue(value)
	{
	}

	ConstantExpression(Scalar value) requires std::same_as<Scalar, bool>
		: ExpressionBase<ConstantExpression<Scalar>>(ValueType::BOOL)
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
class CSL_API InputAttributeExpression : public ExpressionBase<InputAttributeExpression>
{
public:

	InputAttributeExpression(const ValueType& type, uint32_t location, std::string_view name);

	const std::string& name() const;

	uint32_t location() const;

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
class CSL_API OutputAttributeExpression : public ExpressionBase<OutputAttributeExpression>
{
public:

	OutputAttributeExpression(ValueType valueType, uint32_t location, std::string_view name);

	OutputAttributeExpression(ValueType valueType, DefaultSemantics attribute);

	const AttributeBindingInfo& BindingInfo() const;

private:

	AttributeBindingInfo mBindingInfo;
};


class UniformBufferExpression : public ExpressionBase<UniformBufferExpression>
{
public:

	UniformBufferExpression(uint32_t location, std::string_view name);

	const std::string& name() const;

	uint32_t location() const;

private:

	uint32_t mLocation;

	std::string mName;
};


class CSL_API UniformExpression : public ExpressionBase<UniformExpression>
{
public:

	UniformExpression(const ValueType& type, std::string_view name, std::shared_ptr<UniformBufferExpression> buffer);

	const std::string& name() const;

private:

	std::string mName;
};


class CSL_API SamplerExpression : public ExpressionBase<SamplerExpression>
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
	NOT_EQUAL,
	// [] operator
	SUBSCRIPT,
	// ! operator
	NOT,
};


/// Class defining an arithmetic operator expression
class CSL_API OperatorExpression : public ExpressionBase<OperatorExpression>
{
public:

	OperatorExpression(ValueType outputType, Operator op, ExpressionPtr arg0);

	OperatorExpression(ValueType outputType, Operator op, ExpressionPtr arg0, ExpressionPtr arg1);

	Operator getOperator() const;

private:

	Operator mOperator;
};


/// Class defining a cast operator expression
class CSL_API CastExpression : public ExpressionBase<CastExpression>
{
public:

	CastExpression(ValueType outputType, ExpressionPtr input);
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
class CSL_API NativeFunctionExpression : public ExpressionBase<NativeFunctionExpression>
{
public:

	template<typename ...Expressions>
	NativeFunctionExpression(ValueType valueType, NativeFunction function, Expressions&&... inputs)
		: ExpressionBase<NativeFunctionExpression>(valueType, std::vector<ExpressionPtr>{ std::forward<Expressions>(inputs)... })
		, mFunction(function)
	{
	}

	NativeFunction Function() const;

private:

	NativeFunction mFunction;
};


/// Class defining a native constructor call in the shader graph
class CSL_API ConstructorExpression : public ExpressionBase<ConstructorExpression>
{
public:
	using ExpressionBase<ConstructorExpression>::ExpressionBase;
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
class CSL_API SwizzleExpression : public ExpressionBase<SwizzleExpression>
{
public:

	SwizzleExpression(ValueType outputType, Swizzle swizzle, ExpressionPtr input);

	Swizzle swizzle() const;

private:

	Swizzle mSwizzle;
};


class CSL_API IfExpression : public ExpressionBase<IfExpression>
{
public:
	IfExpression(ExpressionPtr condition);
};

class CSL_API ElseIfExpression : public ExpressionBase<ElseIfExpression>
{
public:
	ElseIfExpression(ExpressionPtr condition, std::shared_ptr<IfExpression> input);
	ElseIfExpression(ExpressionPtr condition, std::shared_ptr<ElseIfExpression> input);
};

class CSL_API ElseExpression : public ExpressionBase<ElseExpression>
{
public:
	ElseExpression(std::shared_ptr<IfExpression> input);
	ElseExpression(std::shared_ptr<ElseIfExpression> input);
};

class CSL_API EndIfExpression : public ExpressionBase<EndIfExpression>
{
public:
	EndIfExpression(std::shared_ptr<IfExpression> input);
	EndIfExpression(std::shared_ptr<ElseIfExpression> input);
	EndIfExpression(std::shared_ptr<ElseExpression> input);
};

} // namespace csl

#endif // !CSL_EXPRESSIONS_HPP
