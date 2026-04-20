/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_VECTOR_HPP
#define CSL_VECTOR_HPP

#include <csl/Value.hpp>
#include <csl/Expressions.hpp>

#include <concepts>
#include <type_traits>

namespace csl
{
template<typename T, size_t S> requires (1 <= S && S <= 4)
struct Vector;

template<typename T>
using Scalar = Vector<T, 1>;

/** \brief Type trait that detects whether a type is an instantiation of Vector<T, S>
 *
 * Primary template: defaults to std::false_type for all types. This ensures that non‑Vector types
 * do not accidentally satisfy the trait.
 */
template <typename>
struct TypeTraits : std::false_type {};


template<>
struct TypeTraits<Vector<float, 1>> : std::true_type
{
    using ComponentType                  = float;
    constexpr static size_t Size         = 1;
    constexpr static ValueType ValueType = ValueType::FLOAT;
};


template<>
struct TypeTraits<Vector<float, 2>> : std::true_type
{
    using ComponentType                  = float;
    constexpr static size_t Size         = 2;
    constexpr static ValueType ValueType = ValueType::VEC2F;
};


template<>
struct TypeTraits<Vector<float, 3>> : std::true_type
{
    using ComponentType                  = float;
    constexpr static size_t Size         = 3;
    constexpr static ValueType ValueType = ValueType::VEC3F;
};


template<>
struct TypeTraits<Vector<float, 4>> : std::true_type
{
    using ComponentType                  = float;
    constexpr static size_t Size         = 4;
    constexpr static ValueType ValueType = ValueType::VEC4F;
};


template<>
struct TypeTraits<Vector<int, 1>> : std::true_type
{
    using ComponentType                  = int;
    constexpr static size_t Size         = 1;
    constexpr static ValueType ValueType = ValueType::INT;
};


template<>
struct TypeTraits<Vector<int, 2>> : std::true_type
{
    using ComponentType                  = int;
    constexpr static size_t Size         = 2;
    constexpr static ValueType ValueType = ValueType::VEC2I;
};


template<>
struct TypeTraits<Vector<int, 3>> : std::true_type
{
    using ComponentType                  = int;
    constexpr static size_t Size         = 3;
    constexpr static ValueType ValueType = ValueType::VEC3I;
};


template<>
struct TypeTraits<Vector<int, 4>> : std::true_type
{
    using ComponentType                  = int;
    constexpr static size_t Size         = 4;
    constexpr static ValueType ValueType = ValueType::VEC4I;
};


template<>
struct TypeTraits<Vector<bool, 1>> : std::true_type
{
    using ComponentType                  = bool;
    constexpr static size_t Size         = 1;
    constexpr static ValueType ValueType = ValueType::BOOL;
};





// \brief Concept to ensure V is a csl::Vector of T and size S
template <typename V, typename T, size_t S>
concept VectorType =
    TypeTraits<std::remove_cvref_t<V>>::value     &&
    TypeTraits<std::remove_cvref_t<V>>::Size == S &&
    std::same_as<typename TypeTraits<std::remove_cvref_t<V>>::ComponentType, T>;

// \brief Concept to ensure V is a csl::Vector of T and size S
template <typename V>
concept FloatingPointVector =
    VectorType<V, float, 2> ||
    VectorType<V, float, 3> ||
    VectorType<V, float, 4>;

// \brief Concept to ensure S is any csl::Vector
template<typename V>
concept AnyVector =
    TypeTraits<std::remove_cvref_t<V>>::value &&
    TypeTraits<std::remove_cvref_t<V>>::Size > 1;

// \brief Concept to ensure S is a csl::Scalar of T
template <typename S, typename T>
concept ScalarType =
    TypeTraits<std::remove_cvref_t<S>>::value &&
    TypeTraits<std::remove_cvref_t<S>>::Size == 1 &&
    std::same_as<typename TypeTraits<std::remove_cvref_t<S>>::ComponentType, T>;

// \brief Concept to ensure S is a csl::Scalar of type T
template<typename S>
concept FloatingPointScalar =
    ScalarType<S, float>;

// \brief Concept to ensure S is any csl::Scalar
template<typename S>
concept AnyScalar =
    TypeTraits<std::remove_cvref_t<S>>::value &&
    TypeTraits<std::remove_cvref_t<S>>::Size == 1;

template<typename S, typename T>
concept ConstantType =
    std::same_as<std::remove_cvref_t<S>, T>;

/** \brief Class representing a vector
 *
 */
template<typename T, size_t S> requires (1 <= S && S <= 4)
struct CSL_API Vector : public Value
{
public:

    constexpr static ValueType ValueType = TypeTraits<Vector<T, S>>::ValueType;

    using Value::Value;

    // \brief Default constructor for a scalar
    Vector() requires (S == 1)
        : Vector(Expression::create<ConstantExpression<T>>(T(0)))
    {
    }

    // \brief Default constructor for a 2-component vector
    Vector() requires (S == 2)
        : Vector(T(0), T(0))
    {
    }

    // \brief Default constructor for a 3-component vector
    Vector() requires (S == 3)
        : Vector(T(0), T(0), T(0))
    {
    }

    // \brief Default constructor for a 4-component vector
    Vector() requires (S == 4)
        : Vector(T(0), T(0), T(0), T(0))
    {
    }

    // \brief Construct a scalar from a constant
    Vector(T arg) requires (S == 1)
        : Value(Expression::create<ConstantExpression<T>>(arg))
    {
    }

    // \brief Construct a vector from scalars
    template<typename ...Ts> requires (sizeof...(Ts) == S && S > 1)
        Vector(Ts&&... args)
        : Value(Expression::create<ConstructorExpression>(ValueType,
                                                          Scalar<T>(std::forward<Ts>(args)).expression()...))
    {
    }

    // \brief Construct a vector from a smaller vector with the scalar values appended at the end
    template<size_t S2, typename ...Ts> requires (sizeof...(Ts) + S2 == S)
        Vector(const Vector<T, S2>& v, Ts&&... args)
        : Value(Expression::create<ConstructorExpression>(ValueType,
                                                          v.expression(), 
                                                          Scalar<T>(std::forward<Ts>(args)).expression()...))
    {
    }

    // \brief Copy constructor
    Vector(const Vector& other)
        : Value(Expression::create<ConstructorExpression>(ValueType, other.expression()))
    {
    }

    // \brief Move constructor
    Vector(Vector&& other)
        : Value(std::move(other).expression())
    {
    }

    // \brief Copy assignment operator
    Vector<T, S>& operator=(const Vector<T, S>& other) &
    {
        setExpression(Expression::create<OperatorExpression>(ValueType, 
                                                             Operator::ASSIGNMENT, 
                                                             expression(), 
                                                             other.expression()));
        return *this;
    }

    // \brief Move assignment operator
    Vector<T, S>& operator=(Vector<T, S>&& other) &
    {
        setExpression(Expression::create<OperatorExpression>(ValueType,
                                                             Operator::ASSIGNMENT,
                                                             expression(), 
                                                             other.expression()));
        return *this;
    }

    // \brief Copy assignment operator
    Vector<T, S>& operator=(const Vector<T, S>& other) &&
    {
        expression()->forceInlineResult();
        setExpression(Expression::create<OperatorExpression>(ValueType,
            Operator::ASSIGNMENT,
            expression(),
            other.expression()));
        return *this;
    }

    // \brief Move assignment operator
    Vector<T, S>& operator=(Vector<T, S>&& other) &&
    {
        expression()->forceInlineResult();
        setExpression(Expression::create<OperatorExpression>(ValueType,
            Operator::ASSIGNMENT,
            expression(),
            other.expression()));
        return *this;
    }

    // \brief Get the x component of the vector
    Vector<T, 1> x(this auto&& self) requires(S > 1)
    {
        return { Expression::create<SwizzleExpression>(Vector<T, 1>::ValueType, 
                                                       Swizzle::X,
                                                       std::forward<decltype(self)>(self).expression()) };
    }

    // \brief Get the y component of the vector
    Vector<T, 1> y(this auto&& self) requires(S > 1)
    {
        return { Expression::create<SwizzleExpression>(Vector<T, 1>::ValueType, 
                                                       Swizzle::Y, 
                                                       std::forward<decltype(self)>(self).expression()) };
    }

    // \brief Get the z component of the vector
    Vector<T, 1> z(this auto&& self) requires(S >= 3)
    {
        return { Expression::create<SwizzleExpression>(Vector<T, 1>::ValueType, 
                                                       Swizzle::Z,
                                                       std::forward<decltype(self)>(self).expression()) };
    }

    // \brief Get the w component of the vector
    Vector<T, 1> w(this auto&& self) requires(S >= 4)
    {
        return { Expression::create<SwizzleExpression>(Vector<T, 1>::ValueType, 
                                                       Swizzle::W, 
                                                       std::forward<decltype(self)>(self).expression()) };
    }

    // \brief Get the xy components of the vector
    Vector<T, 2> xy(this auto&& self)
    {
        return { Expression::create<SwizzleExpression>(Vector<T, 2>::ValueType, 
                                                       Swizzle::XY, 
                                                       std::forward<decltype(self)>(self).expression()) };
    }

    // \brief Get the xyz components of the vector
    Vector<T, 3> xyz(this auto&& self) requires(S >= 3)
    {
        return { Expression::create<SwizzleExpression>(Vector<T, 3>::ValueType, 
                                                       Swizzle::XYZ, 
                                                       std::forward<decltype(self)>(self).expression())};
    }

    Scalar<T> operator[](int i) const & requires (S > 1)
    {
        return { Expression::create<OperatorExpression>(Scalar<T>::ValueType, 
                                                        Operator::SUBSCRIPT,
                                                        expression(),
                                                        Scalar<int>(i).expression()) };
    }

    template <ScalarType<int> T>
    Scalar<T> operator[](T&& i) const & requires (S > 1)
    {
        return { Expression::create<OperatorExpression>(Scalar<T>::ValueType,
                                                        Operator::SUBSCRIPT,
                                                        expression(),
                                                        Scalar<int>(i).expression()) };
    }

    Scalar<T> operator[](int i) && requires (S > 1)
    {
        auto node = std::forward<Value>(*this).expression();
        return { Expression::create<OperatorExpression>(Scalar<T>::ValueType, 
                                                        Operator::SUBSCRIPT,
                                                        node,
                                                        Scalar<int>(i).expression()) };
    }

    template <ScalarType<int> T>
    Scalar<T> operator[](T&& i) && requires (S > 1)
    {
        return { Expression::create<OperatorExpression>(Scalar<T, 3>::ValueType, 
                                                        Operator::SUBSCRIPT,
                                                        std::forward<decltype(*this)>(*this).expression(),
                                                        std::forward<T>(i).expression()) };
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Multiplication operator
    // ----------------------------------------------------------------------------------------------------------------

    // \brief Multiplication operator
    template<typename LHS, typename RHS>
    requires VectorType<LHS, T, S> && VectorType<RHS, T, S> ||
             VectorType<LHS, T, S> && VectorType<RHS, T, 1> ||
             VectorType<LHS, T, 1> && VectorType<RHS, T, S>
    friend Vector<T, S> operator*(LHS&& lhs, RHS&& rhs)
    {
        return { Expression::create<OperatorExpression>(Vector<T, S>::ValueType, 
                                                        Operator::MULTIPLY,
                                                        std::forward<LHS>(lhs).expression(),
                                                        std::forward<RHS>(rhs).expression()) };
    }

    // \brief vector * constant operator
    template<VectorType<T, S> LHS>
    friend Vector<T, S> operator*(LHS&& lhs, T&& rhs)
    {
        return std::forward<LHS>(lhs) * Vector<T, 1>(rhs);
    }

    // \brief constant * vector operator
    template<VectorType<T, S> RHS>
    friend Vector<T, S> operator*(T&& lhs, RHS&& rhs)
    {
        return Vector<T, 1>(lhs) * std::forward<RHS>(rhs);
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Division operator
    // ----------------------------------------------------------------------------------------------------------------

    // \brief Multiplication operator
    template<typename LHS, typename RHS>
    requires VectorType<LHS, T, S> && VectorType<RHS, T, S> ||
             VectorType<LHS, T, S> && VectorType<RHS, T, 1> ||
             VectorType<LHS, T, 1> && VectorType<RHS, T, S>
    friend Vector<T, S> operator/(LHS&& lhs, RHS&& rhs)
    {
        return { Expression::create<OperatorExpression>(Vector<T, S>::ValueType, 
                                                        Operator::DIVIDE,
                                                        std::forward<LHS>(lhs).expression(),
                                                        std::forward<RHS>(rhs).expression()) };
    }

    // \brief vector / constant operator
    template<VectorType<T, S> LHS>
    friend Vector<T, S> operator/(LHS&& lhs, T&& rhs)
    {
        return std::forward<LHS>(lhs) / Vector<T, 1>(rhs);
    }

    // \brief constant / vector operator
    template<VectorType<T, S> RHS>
    friend Vector<T, S> operator/(T&& lhs, RHS&& rhs)
    {
        return Vector<T, 1>(lhs) / std::forward<RHS>(rhs);
    }
};


template<typename LHS, typename RHS>
concept SameScalars = AnyScalar<LHS> && 
                      AnyScalar<RHS> && 
                      std::same_as<typename TypeTraits<std::remove_cvref_t<LHS>>::ComponentType,
                                   typename TypeTraits<std::remove_cvref_t<RHS>>::ComponentType>;

template<typename LHS, typename RHS>
concept ScalarAndConstant = AnyScalar<LHS> &&
                            std::same_as<typename TypeTraits<std::remove_cvref_t<LHS>>::ComponentType,
                                         std::remove_cvref_t<RHS>>;

template<typename LHS, typename RHS>
concept ConstantAndScalar = AnyScalar<RHS> &&
                            std::same_as<std::remove_cvref_t<LHS>, 
                                         typename TypeTraits<std::remove_cvref_t<RHS>>::ComponentType>;

// ----------------------------------------------------------------------------------------------------------------
// GREATER comparison operator
// ----------------------------------------------------------------------------------------------------------------

template<typename LHS, typename RHS>
requires SameScalars<LHS, RHS>
Scalar<bool> operator>(LHS&& lhs, RHS&& rhs)
{
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::GREATER,
                                                    std::forward<LHS>(lhs).expression(),
                                                    std::forward<RHS>(rhs).expression() )};
}

template<typename LHS, typename RHS>
requires ScalarAndConstant<LHS, RHS>
Scalar<bool> operator>(LHS&& lhs, RHS&& rhs)
{
    using T = std::remove_cvref_t<RHS>;
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::GREATER,
                                                    std::forward<LHS>(lhs).expression(),
                                                    Scalar<T>(rhs).expression()) };
}

template<typename LHS, typename RHS>
requires ConstantAndScalar<LHS, RHS>
Scalar<bool> operator>(LHS&& lhs, RHS&& rhs)
{
    using T = std::remove_cvref_t<RHS>;
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::GREATER,
                                                    std::forward<LHS>(lhs).expression(),
                                                    Scalar<T>(rhs).expression()) };
}

// ----------------------------------------------------------------------------------------------------------------
// LESS comparison operator
// ----------------------------------------------------------------------------------------------------------------

template<typename LHS, typename RHS>
requires SameScalars<LHS, RHS>
Scalar<bool> operator<(LHS&& lhs, RHS&& rhs)
{
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::LESS,
                                                    std::forward<LHS>(lhs).expression(),
                                                    std::forward<RHS>(rhs).expression()) };
}

template<typename LHS, typename RHS>
requires ScalarAndConstant<LHS, RHS>
Scalar<bool> operator<(LHS&& lhs, RHS&& rhs)
{
    using T = std::remove_cvref_t<RHS>;
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::LESS,
                                                    std::forward<LHS>(lhs).expression(),
                                                    Scalar<T>(rhs).expression()) };
}

template<typename LHS, typename RHS>
requires ConstantAndScalar<LHS, RHS>
Scalar<bool> operator<(LHS&& lhs, RHS&& rhs)
{
    using T = std::remove_cvref_t<RHS>;
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::LESS,
                                                    std::forward<LHS>(lhs).expression(),
                                                    Scalar<T>(rhs).expression()) };
}

// ----------------------------------------------------------------------------------------------------------------
// GEQUAL comparison operator
// ----------------------------------------------------------------------------------------------------------------

template<typename LHS, typename RHS>
requires SameScalars<LHS, RHS>
Scalar<bool> operator>=(LHS&& lhs, RHS&& rhs)
{
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::GREATER_OR_EQUAL,
                                                    std::forward<LHS>(lhs).expression(),
                                                    std::forward<RHS>(rhs).expression()) };
}

template<typename LHS, typename RHS>
requires ScalarAndConstant<LHS, RHS>
Scalar<bool> operator>=(LHS&& lhs, RHS&& rhs)
{
    using T = std::remove_cvref_t<RHS>;
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::GREATER_OR_EQUAL,
                                                    std::forward<LHS>(lhs).expression(),
                                                    Scalar<T>(rhs).expression()) };
}

template<typename LHS, typename RHS>
requires ConstantAndScalar<LHS, RHS>
Scalar<bool> operator>=(LHS&& lhs, RHS&& rhs)
{
    using T = std::remove_cvref_t<RHS>;
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::GREATER_OR_EQUAL,
                                                    std::forward<LHS>(lhs).expression(),
                                                    Scalar<T>(rhs).expression()) };
}

// ----------------------------------------------------------------------------------------------------------------
// LEQUAL comparison operator
// ----------------------------------------------------------------------------------------------------------------

template<typename LHS, typename RHS>
requires SameScalars<LHS, RHS>
Scalar<bool> operator<=(LHS && lhs, RHS && rhs)
{
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType,
                                                    Operator::LESS_OR_EQUAL,
                                                    std::forward<LHS>(lhs).expression(),
                                                    std::forward<RHS>(rhs).expression()) };
}

template<typename LHS, typename RHS>
requires ScalarAndConstant<LHS, RHS>
Scalar<bool> operator<=(LHS&& lhs, RHS&& rhs)
{
    using T = std::remove_cvref_t<RHS>;
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::LESS_OR_EQUAL,
                                                    std::forward<LHS>(lhs).expression(),
                                                    Scalar<T>(rhs).expression()) };
}

template<typename LHS, typename RHS>
requires ConstantAndScalar<LHS, RHS>
Scalar<bool> operator<=(LHS&& lhs, RHS&& rhs)
{
    using T = std::remove_cvref_t<RHS>;
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::LESS_OR_EQUAL,
                                                    std::forward<LHS>(lhs).expression(),
                                                    Scalar<T>(rhs).expression()) };
}

// ----------------------------------------------------------------------------------------------------------------
// EQUAL comparison operator
// ----------------------------------------------------------------------------------------------------------------

template<typename LHS, typename RHS>
requires SameScalars<LHS, RHS>
Scalar<bool> operator==(LHS&& lhs, RHS&& rhs)
{
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::EQUAL,
                                                    std::forward<LHS>(lhs).expression(),
                                                    std::forward<RHS>(rhs).expression()) };
}

template<typename LHS, typename RHS>
requires ScalarAndConstant<LHS, RHS>
Scalar<bool> operator==(LHS&& lhs, RHS&& rhs)
{
    using T = std::remove_cvref_t<RHS>;
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType,
                                                    Operator::EQUAL,
                                                    std::forward<LHS>(lhs).expression(),
                                                    Scalar<T>(rhs).expression()) };
}

template<typename LHS, typename RHS>
requires ConstantAndScalar<LHS, RHS>
Scalar<bool> operator==(LHS&& lhs, RHS&& rhs)
{
    using T = std::remove_cvref_t<RHS>;
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType,
                                                    Operator::EQUAL,
                                                    std::forward<LHS>(lhs).expression(),
                                                    Scalar<T>(rhs).expression()) };
}

// ----------------------------------------------------------------------------------------------------------------
// NEQUAL comparison operator
// ----------------------------------------------------------------------------------------------------------------

template<typename LHS, typename RHS>
requires SameScalars<LHS, RHS>
Scalar<bool> operator!=(LHS&& lhs, RHS&& rhs)
{
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::NOT_EQUAL,
                                                    std::forward<LHS>(lhs).expression(),
                                                    std::forward<RHS>(rhs).expression()) };
}

template<typename LHS, typename RHS>
requires ScalarAndConstant<LHS, RHS>
Scalar<bool> operator!=(LHS&& lhs, RHS&& rhs)
{
    using T = std::remove_cvref_t<RHS>;
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::NOT_EQUAL,
                                                    std::forward<LHS>(lhs).expression(),
                                                    Scalar<T>(rhs).expression()) };
}

template<typename LHS, typename RHS>
requires ConstantAndScalar<LHS, RHS>
Scalar<bool> operator!=(LHS&& lhs, RHS&& rhs)
{
    using T = std::remove_cvref_t<RHS>;
    return { Expression::create<OperatorExpression>(Scalar<bool>::ValueType, 
                                                    Operator::NOT_EQUAL,
                                                    std::forward<LHS>(lhs).expression(),
                                                    Scalar<T>(rhs).expression()) };
}

using Vec2F = Vector<float, 2>;
using Vec3F = Vector<float, 3>;
using Vec4F = Vector<float, 4>;

using Vec2I = Vector<int, 2>;
using Vec3I = Vector<int, 3>;
using Vec4I = Vector<int, 4>;

using Float = Scalar<float>;
using Int   = Scalar<int>;
using Bool  = Scalar<bool>;

} // namespace csl 

#endif // !CSL_VECTOR_HPP
