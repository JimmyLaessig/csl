/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#include <csl/Attribute.hpp>

#include <csl/Expressions.hpp>
#include <csl/Node.hpp>

#include <cassert>
#include <ranges>
#include <set>

using namespace csl;


ExpressionPtr
csl::RegisterAttribute(ValueType type, uint32_t location, std::string_view name)
{
    auto current = csl::Attributes::current();

    if (!current)
    {
        return nullptr;
    }

    if (current->isInput())
    {
        return Expression::create<csl::InputAttributeExpression>(type, location, name);
    }
    else
    {
        return Expression::create<csl::OutputAttributeExpression>(type, location, name);
    }
}


ExpressionPtr
csl::RegisterAttribute(ValueType type, DefaultSemantics semantic)
{
    auto current = csl::Attributes::current();

    if (!current)
    {
        return nullptr;
    }

    if (current->isInput())
    {
        return nullptr;
    }
    else
    {
        return Expression::create<csl::OutputAttributeExpression>(type, semantic);
    }
}
