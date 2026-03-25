#ifndef CSL_CONTROLFLOW_HPP
#define CSL_CONTROLFLOW_HPP

#include <csl/Scalar.hpp>

namespace csl
{

//template<typename T>
//struct Then
//{
//public:
//	T Else(T value)
//	{
//		return T{ ShaderModule::current()->addExpression<ConditionalExpression>(mCondition.node(), mIf.node(), value.node()) };
//	}
//
//	Then(const Then& other) = delete;
//	Then(Then&& other) = delete;
//
//	Then& operator=(Then&& other) = delete;
//	Then& operator=(const Then& other) = delete;
//
//private:
//
//	friend class If;
//
//	Then(Bool condition, T ifCond)
//		: mCondition(condition)
//		, mIf(ifCond)
//	{
//	}
//
//	std::shared_ptr<Expression> mCondition;
//	T mIf;
//};
//
//struct If
//{
//public:
//
//	If(Bool condition)
//		: mCondition(condition.node())
//	{
//	}
//
//	template<typename T>
//	Then<T> Then(T value)
//	{
//		return Then<T>(mCondition, value);
//	}
//
//	If(const If& other) = delete;
//	If(If&& other) = delete;
//
//	If& operator=(If&& other) = delete;
//	If& operator=(const If& other) = delete;
//
//private:
//
//	std::shared_ptr<Expression> mCondition;
//
//};

} // namespace ShaderLanguage 

#endif // !CSL_CONTROLFLOW_HPP
