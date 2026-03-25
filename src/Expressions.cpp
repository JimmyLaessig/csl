#include <csl/Expressions.hpp>

#include <csl/Node.hpp>

using namespace csl;


OutputAttributeExpression::OutputAttributeExpression(ValueType valueType, uint32_t location, std::string_view name)
	: ExpressionBase(valueType)
	, mBindingInfo(AttributeBinding{ std::string(name), location })
{
}


OutputAttributeExpression::OutputAttributeExpression(ValueType valueType, DefaultSemantics attribute)
	: ExpressionBase(valueType)
	, mBindingInfo(attribute)
{
}


const AttributeBindingInfo&
OutputAttributeExpression::BindingInfo() const
{ 
	return mBindingInfo;
}
