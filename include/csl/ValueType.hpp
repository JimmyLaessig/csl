/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_VALUETYPE_HPP
#define CSL_VALUETYPE_HPP

namespace csl
{

enum class ValueType
{
	/// Conditional type
	BOOL,
	/// 32-bit signed integer
	INT,
	/// 2-component 32-bit signed integer vector
	VEC2I,
	/// 3-component 32-bit signed integer vector
	VEC3I,
	/// 4-component 32-bit signed integer vector
	VEC4I,
	/// 32-bit single-precision floating-point number
	FLOAT,
	/// 2-component 32-bit single-precision floating-point vector
	VEC2F,
	/// 3-component 32-bit single-precision floating-point vector
	VEC3F,
	/// 4-component 32-bit single-precision floating-point vector
	VEC4F,
	/// 3x3 32-bit single-precision floating-point matrix
	MAT33F,
	/// 4x4 32-bit single-precision floating-point matrix
	MAT44F,
	/// 2D texture sampler
	SAMPLER2D,
	// Void type
	VOID,
};

} // namespace csl 

#endif // !CSL_VALUETYPE_HPP
