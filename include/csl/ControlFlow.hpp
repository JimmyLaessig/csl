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

#include <concepts>

namespace csl
{

template <typename F>
concept NullaryVoidFunction = std::invocable<F> &&
                              std::same_as<std::invoke_result_t<F>, void>;


struct ConditionBuilder
{
public:

    ConditionBuilder(const ConditionBuilder&) = delete;
    ConditionBuilder(ConditionBuilder&&) = delete;

    ConditionBuilder& operator=(const ConditionBuilder&) = delete;
    ConditionBuilder& operator=(ConditionBuilder&&) = delete;

    template<VectorType<bool, 1> Cond, NullaryVoidFunction F>
    ConditionBuilder(Cond&& condition, F&& func)
    {
        //Node::create(IfExpression(), std::forward<Cond>(condition).node());
        //Node::create(BeginScopeExpression());
        //func();
        //Node::create(EndScopeExpression());
    }

    template<VectorType<bool, 1> Cond, NullaryVoidFunction F>
    ConditionBuilder& ElseIf(Cond&& condition, F&& func)
    {
        /*Node::create(ElseIfExpression(), std::forward<Cond>(condition).node());
        Node::create(BeginScopeExpression());
        func();
        Node::create(EndScopeExpression());*/
        return *this;
    }

    template<NullaryVoidFunction F>
    void Else(F&& func)
    {
        //Node::create(ElseExpression());
        //Node::create(BeginScopeExpression());
        //func();
        //Node::create(EndScopeExpression());
    }

private:

    ConditionBuilder() = default;
};


template<VectorType<bool, 1> Cond, NullaryVoidFunction F>
ConditionBuilder If(Cond&& condition, F&& then)
{
    return ConditionBuilder(std::forward<Cond&&>(condition), std::forward<F&&>(then));
}

} // namespace csl 

#endif // !CSL_CONTROLFLOW_HPP
