/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_VISITOR_HPP
#define CSL_VISITOR_HPP

// Visitor class definition for std::visit
template<class... Ts>
struct Visitor : Ts...
{
	using Ts::operator()...;
};

#endif // !CSL_VISITOR_HPP