/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_FUNCTIONS_HPP
#define CSL_FUNCTIONS_HPP

#include <csl/Matrix.hpp>

#include <concepts>

namespace csl
{

/** \brief Calculate the unit vector in the same direction as the original vector
 *
 * \param v The vector to normalize
 * \return The normalized vector
 */
template<FloatingPointVector Vec>
auto normalize(Vec&& v)
{
    using ReturnType = std::remove_cvref_t<Vec>;

    return ReturnType{ Node::create(NativeFunctionExpression(TypeTraits<ReturnType>::ValueType, NativeFunction::NORMALIZE),
                                    std::forward<Vec>(v).node()) };
}


/** \brief Calculate the dot product of two vectors
 *
 * \param v1 The first vector
 * \param v2 The second vector
 * \return The dot product of v1 and v2
 */
template<FloatingPointVector Vec1, FloatingPointVector Vec2>
Scalar<float> dot(Vec1&& v1, Vec2&& v2)
{
    return { Node::create(NativeFunctionExpression(ValueType::FLOAT, NativeFunction::DOT),
                          std::forward<Vec1>(v1).node(),
                          std::forward<Vec2>(v2).node()) };
}


/** \brief Calculate the cross product of two vectors
 *
 * \param v1 The first vector
 * \param v2 The second vector
 * \return The cross product of b1 and b2
 */
template<VectorType<float, 3> Vec1, VectorType<float, 3> Vec2>
Vector<float, 3> cross(Vec1&& v1, Vec2&& v2)
{
    return { Node::create(NativeFunctionExpression(Vector<float, 3>::ValueType, NativeFunction::CROSS),
                          std::forward<Vec1>(v1).node(), 
                          std::forward<Vec2>(v2).node()) };
}


/** \brief Calculate the length of the vector
 *
 * \param v The vector of which to calculate the length
 * \return The length of v
 */
template<FloatingPointVector Vec>
Scalar<float> length(Vec&& v)
{
    return { Node::create(NativeFunctionExpression(Scalar<float>::ValueType, NativeFunction::LENGTH),
                          std::forward<Vec>(v).node()) };
}


/** \brief Calculate the distance between the two points
 *
 * \param p1 The first point
 * \param p2 The second point
 * \return The distance between p1 and p2
 */
template<FloatingPointVector Vec1, FloatingPointVector Vec2>
Scalar<float> distance(Vec1&& p1, Vec2&& p2)
{
    return { Node::create(NativeFunctionExpression(Scalar<float>::ValueType, NativeFunction::DISTANCE),
                          std::forward<Vec1>(p1).node(),
                          std::forward<Vec2>(p2).node()) };
}


/** \brief Calculate the square root of the parameter
 *
 * \param v The value of which to take the square root
 * \return The square root of v
 */
template<FloatingPointScalar T>
Scalar<float> sqrt(T&& v)
{
    return { Node::create(NativeFunctionExpression(Scalar<float>::ValueType, NativeFunction::SQRT),
                          std::forward<T>(v).node()) };
}


/** \brief Calculate the inverse of the square root of the parameter
 *
 * \param v The value of which to take the inverse square root
 * \return The inverse square root of v
 */
template<FloatingPointScalar T>
Scalar<float> inverseSqrt(T&& v)
{
    return { Node::create(NativeFunctionExpression(Scalar<float>::ValueType, NativeFunction::INVERSESQRT),
                          std::forward<T>(v).node()) };
}


/** \brief Calculate the value of the first parameter raised to the power of the second
 * 
 * \param base The base or the power
 * \param exp The exponent of the power
 * \return The value of base raised to the value of exp
 */ 
template<AnyScalar T1, AnyScalar T2>
auto pow(T1&& base, T2&& exp)
{
    using ReturnType = std::remove_cvref_t<T1>;
    return { Node::create(NativeFunctionExpression(Scalar<float>::ValueType, NativeFunction::SQRT),
                          std::forward<T1>(base).node(),
                          std::forward<T2>(exp).node()) };
}


/** \brief Get the sine of the angle
 *
 * \param angle The angle in radians
 */
template<FloatingPointScalar T>
Scalar<float> sin(T&& angle)
{
    return { Node::create(NativeFunctionExpression(Scalar<float>::ValueType, NativeFunction::SIN),
                          std::forward<T>(angle).node()) };
}


/** \brief Get the cosine of the angle
 *
 * \param angle The angle in radians
 */
template<FloatingPointScalar T>
Scalar<float> cos(T&& angle)
{
    return { Node::create(NativeFunctionExpression(Scalar<float>::ValueType, NativeFunction::COS),
                          std::forward<T>(angle).node()) };
}


/** \brief Get the tangent of the angle
 *
 * \param angle The angle in radians
 */
template<FloatingPointScalar T>
Scalar<float> tan(T&& angle)
{
    return { Node::create(NativeFunctionExpression(Scalar<float>::ValueType, NativeFunction::TAN),
                          std::forward<T>(angle).node()) };
}


/** \brief Get the arcsine of the angle
 *
 * \param angle The angle in radians
 */
template<FloatingPointScalar T>
Scalar<float> asin(T&& angle)
{
    return { Node::create(NativeFunctionExpression(Scalar<float>::ValueType, NativeFunction::ASIN),
                          std::forward<T>(angle).node()) };
}


/** \brief Get the arc cosine of the angle
 *
 * \param angle The angle in radians
 */
template<FloatingPointScalar T>
Scalar<float> acos(T&& angle)
{
    return { Node::create(NativeFunctionExpression(Scalar<float>::ValueType, NativeFunction::ACOS),
                          std::forward<T>(angle).node()) };
}


/** \brief Get the arc tangent of the parameter
 *
 * \param y Specify the numerator of the fraction whose arctangent to return.
 * \param x Specify the denominator of the fraction whose arctangent to return.
 */
template<FloatingPointScalar T1, FloatingPointScalar T2>
Scalar<float> atan(T1&& y, T2&& x)
{
    return { Node::create(NativeFunctionExpression(Scalar<float>::ValueType, NativeFunction::ATAN),
                          std::forward<T1>(y).node(),
                          std::forward<T2>(x).node()) };
}


/** \brief Discard the current pixel
 * This function is only available in the fragment shader
 */
inline void discard()
{
    //Node::create(NativeFunctionExpression(ValueType::VOID, NativeFunction::DISCARD));
}

} // namespace csl 

#endif // !CSL_FUNCTIONS_HPP
