/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_MATRIX_HPP
#define CSL_MATRIX_HPP

#include <csl/Vector.hpp>

namespace csl
{

template<typename T, size_t C, size_t R> requires (C == R) && (C == 3 || C == 4)
struct Matrix;

template<>
struct TypeTraits<Matrix<float, 3, 3>> : std::true_type
{
	using ComponentType          = float;
	constexpr static size_t C = 3;
	constexpr static size_t R = 3;
	constexpr static ValueType ValueType = ValueType::MAT33F;
};

template<>
struct TypeTraits<Matrix<float, 4, 4>> : std::true_type
{
	using ComponentType       = float;
	constexpr static size_t C = 4;
	constexpr static size_t R = 4;
	constexpr static ValueType ValueType = ValueType::MAT44F;
};

// \brief Concept to ensure M is a csl::Matrix of T with C columns and R rows
template<typename M, typename T, size_t C, size_t R>
concept MatrixType = 
	TypeTraits<std::remove_cvref_t<M>>::value &&
	TypeTraits<std::remove_cvref_t<M>>::C == C &&
	TypeTraits<std::remove_cvref_t<M>>::R == R &&
	std::same_as<typename TypeTraits<std::remove_cvref_t<M>>::ComponentType, T>;

// \brief Class representing a matrix value
template<typename T, size_t C, size_t R> requires (C == R) && (C == 3 || C == 4)
struct CSL_API  Matrix : public Value
{
public:

	Matrix() 
		: Matrix(T(1))
	{}

	using Value::Value;

	static constexpr ValueType ValueType = TypeTraits<Matrix<T, C, R>>::ValueType;

	/// Create a new Matrix with the diagonal filled with the value
	Matrix(T v)  requires (C == 3)
		: Matrix(v, 0, 0,
			     0, v, 0,
			     0, 0, v)
	{
	}


	Matrix(T v) requires (C == 4)
		: Matrix(v, 0, 0, 0,
			     0, v, 0, 0,
			     0, 0, v, 0,
			     0, 0, 0, v)
	{
	}


	template<typename ...Ts> requires (sizeof...(Ts) == C * R)
	Matrix(Ts... args)
		: Value(Expression::create<ConstructorExpression>(Matrix::ValueType, Scalar<T>(args).expression()...))
	{
	}

	Matrix(const Matrix& other)
		: Value(Expression::create<ConstructorExpression>(Matrix::ValueType, other.expression()))
	{
	}

	Matrix(Matrix&& other)
		: Value(Expression::create<ConstructorExpression>(Matrix::ValueType, other.expression()))
	{
	}

	Matrix& operator=(const Matrix& other)
	{
		setExpression(Expression::create<OperatorExpression>(Matrix::ValueType, 
			                                                 Operator::ASSIGNMENT, 
			                                                 expression(), 
			                                                 other.expression()));
		return *this;
	}

	Matrix& operator=(Matrix&& other)
	{
		setExpression(Expression::create<OperatorExpression>(Matrix::ValueType,
			                                                 Operator::ASSIGNMENT, 
			                                                 expression(),
			                                                 other.expression()));
		return *this;
	}

	/// Matrix multiplication operator
	template<typename LHS, typename RHS>
	friend Vector<T, C> operator*(LHS&& lhs, RHS&& rhs)
		requires (MatrixType<LHS, T, C, R> && MatrixType<RHS, T, C, R>) ||
		         (MatrixType<LHS, T, C, R> && VectorType<RHS, T, C>)    ||
				 (VectorType<LHS, T, C>    && MatrixType<RHS, T, C, R>)
	{
		return { Expression::create<OperatorExpression>(Vector<T, C>::ValueType, 
			                                            Operator::MULTIPLY,
							                            std::forward<LHS>(lhs).expression(),
							                            std::forward<RHS>(rhs).expression()) };
	}
};

using Mat33F = Matrix<float, 3, 3>;
using Mat44F = Matrix<float, 4, 4>;

} // namespace csl 

#endif // !CSL_MATRIX_HPP