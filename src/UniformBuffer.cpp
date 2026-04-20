/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#include <csl/UniformBuffer.hpp>

#include <csl/Expressions.hpp>
#include <csl/Node.hpp>

#include <cassert>
#include <ranges>
#include <set>

using namespace csl;

namespace
{

static UniformBufferBase* sCurrent = nullptr;

} // namespace

UniformBufferBase::UniformBufferBase(uint32_t location, std::string_view name)
    : mExpression(Expression::create<UniformBufferExpression>(location, name))
{
}


void
UniformBufferBase::beginScope(UniformBufferBase& uniformBuffer)
{
    sCurrent = &uniformBuffer;
}


void
UniformBufferBase::endScope()
{
    sCurrent = nullptr;
}


std::shared_ptr<UniformBufferExpression>
UniformBufferBase::expression() const
{
    return mExpression;
}


ExpressionPtr
csl::RegisterUniform(ValueType type, std::string_view name)
{
    if (auto current = sCurrent)
    {
        return Expression::create<UniformExpression>(type, name, current->expression());
    }

    return nullptr;
}
