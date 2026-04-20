/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_CONTROLFLOW_HPP
#define CSL_CONTROLFLOW_HPP

#include <csl/Vector.hpp>
#include <csl/Visitor.hpp>

#include <csl/Node.hpp>

#include <concepts>

namespace csl
{

template <typename F>
concept NullaryVoidFunction = std::invocable<F> &&
                              std::same_as<std::invoke_result_t<F>, void>;

namespace detail
{


struct ConditionBuilder
{
public:

    template<VectorType<bool, 1> Cond, NullaryVoidFunction F>
    ConditionBuilder(Cond&& condition, F&& func)
    {
        mExpression = Expression::create<IfExpression>(std::forward<Cond>(condition).expression());
        func();
    }

    template<VectorType<bool, 1> Cond, NullaryVoidFunction F>
    ConditionBuilder& ElseIf(Cond&& condition, F&& func)
    {
        std::visit(Visitor{ [&](auto expr)
        {
            mExpression = Expression::create<ElseIfExpression>(std::forward<Cond>(condition).expression(),
                                                               std::move(expr));
        } }, mExpression);
       
        func();
        return *this;
    }

    template<NullaryVoidFunction F>
    void Else(F&& func)
    {
        std::visit(Visitor{ [&](auto expr)
        {
            Expression::create<ElseExpression>(expr);
        } }, mExpression);

        func();
    }

    ~ConditionBuilder()
    {
        std::visit(Visitor{ [&](auto expr)
        {
            if (expr)
            {
                Expression::create<EndIfExpression>(expr);
            }
        } }, mExpression);
    }

private:

    std::variant<std::shared_ptr<IfExpression>, std::shared_ptr<ElseIfExpression>> mExpression;

};

} // namespace detail

template<VectorType<bool, 1> Cond, NullaryVoidFunction F>
detail::ConditionBuilder If(Cond&& condition, F&& then)
{
    return detail::ConditionBuilder(std::forward<Cond>(condition), std::forward<F>(then));
}

} // namespace csl 

#endif // !CSL_CONTROLFLOW_HPP
