/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#include <csl/CompilerGLSL.hpp>

#include <csl/Visitor.hpp>
#include <csl/Node.hpp>

#include <cassert>
#include <cmath>
#include <format>
#include <map>
#include <ranges>
#include <set>
#include <sstream>
#include <string_view>
#include <utility>


using namespace csl;

namespace
{

constexpr std::string_view
getTypeShortName(ValueType typeId)
{
	switch (typeId)
	{
		case ValueType::BOOL:      return "b";
		case ValueType::INT:       return "i";
		case ValueType::VEC2I:      return "iv";
		case ValueType::VEC3I:      return "iv";
		case ValueType::VEC4I:      return "iv";
		case ValueType::FLOAT:     return "f";
		case ValueType::VEC2F:    return "v";
		case ValueType::VEC3F:    return "v";
		case ValueType::VEC4F:    return "v";
		case ValueType::MAT33F:  return "m";
		case ValueType::MAT44F:  return "m";
		case ValueType::SAMPLER2D: return "s";
	}

	std::unreachable();
}


constexpr std::string_view
toString(ValueType typeId)
{
	switch (typeId)
	{
		case ValueType::BOOL:      return "bool";
		case ValueType::INT:       return "int";
		case ValueType::VEC2I:      return "ivec2";
		case ValueType::VEC3I:      return "ivec3";
		case ValueType::VEC4I:      return "ivec5";
		case ValueType::FLOAT:     return "float";
		case ValueType::VEC2F:    return "vec2";
		case ValueType::VEC3F:    return "vec3";
		case ValueType::VEC4F:    return "vec4";
		case ValueType::MAT33F:  return "mat3";
		case ValueType::MAT44F:  return "mat4";
		case ValueType::SAMPLER2D: return "sampler2D";
	}

	std::unreachable();
}


constexpr std::string_view
toString(Operator op)
{
	switch (op)
	{
	    case Operator::ASSIGNMENT:       return "=";
		case Operator::MULTIPLY:         return "*";
		case Operator::DIVIDE:           return "/";
		case Operator::ADD:	             return "+";
		case Operator::SUBTRACT:         return "-";
		case Operator::EQUAL:            return "==";
		case Operator::NOT_EQUAL:        return "!=";
		case Operator::GREATER:		     return ">";
		case Operator::LESS:			 return "<";
		case Operator::GREATER_OR_EQUAL: return ">=";
		case Operator::LESS_OR_EQUAL:    return "<=";
	}

	std::unreachable();
}


constexpr std::string_view
toString(NativeFunction func)
{
	switch (func)
	{
		case NativeFunction::NORMALIZE:    return "normalize";
		case NativeFunction::DOT:          return "dot";
		case NativeFunction::CROSS:        return "cross";
		case NativeFunction::LENGTH:       return "length";
		case NativeFunction::DISTANCE:     return "distance";
		case NativeFunction::SAMPLE:       return "texture";
		case NativeFunction::TEXTURE_SIZE: return "textureSize";
	}
	std::unreachable();
}


constexpr std::string_view
toString(Swizzle swizzle)
{
	switch (swizzle)
	{
		case Swizzle::X:   return "x";
		case Swizzle::Y:   return "y";
		case Swizzle::Z:   return "z";
		case Swizzle::W:   return "w";
		case Swizzle::XY:  return "xy";
		case Swizzle::XYZ: return "xyz";
	}
	std::unreachable();
}


constexpr auto TAB = "    ";

} // namespace


std::string
CompilerGLSL::format(const ConstantExpression<float>& expr)
{
	float integral;
	std::string suffix = std::modf(expr.value(), &integral) == 0 ? ".f" : "f";
	return std::format("{}{}", expr.value(), suffix);
}


std::string
CompilerGLSL::format(const ConstantExpression<int>& expr)
{
	return std::to_string(expr.value());
}


std::string
CompilerGLSL::format(const ConstantExpression<bool>& expr)
{
	return expr.value() ? "true" : "false";
}


std::string
CompilerGLSL::format(const InputAttributeExpression& expr)
{
	return expr.name();
}


std::string
CompilerGLSL::format(const OutputAttributeExpression& expr)
{
	return std::visit(Visitor{
		[](DefaultSemantics semantic) -> std::string
		{
			switch (semantic)
			{
				case DefaultSemantics::SV_POSITION: return "gl_Position";
				case DefaultSemantics::SV_DEPTH:    return "gl_FragDepth";
				default: assert(false); return "";
			}
		},
		[](const AttributeBinding& binding)
		{
			return std::format("out_{}", binding.name);
		} }, expr.BindingInfo());
}


std::string
CompilerGLSL::format(const UniformExpression& expr)
{
	return expr.name();
}


std::string
CompilerGLSL::format(const OperatorExpression& expr)
{
	auto inputs = expr.node()->inputs();
	if (expr.getOperator() == Operator::ASSIGNMENT)
	{
		auto lhs = resolve(*inputs[0]);
		mNameLookUp[expr.node().get()] = lhs;
		return std::format("{} {} {}",
			               lhs,
			               toString(expr.getOperator()),
			               resolve(*inputs[1]));
	}
	else
	{
		return std::format("({} {} {})",
			               resolve(*inputs[0]),
			               toString(expr.getOperator()),
			               resolve(*inputs[1]));
	}
}


std::string
CompilerGLSL::format(const NativeFunctionExpression& expr)
{
	return std::format("{}({})",
		               toString(expr.Function()),
					   formatFunctionArgumentList(expr.node()->inputs()));
}


std::string
CompilerGLSL::format(const ConstructorExpression& expr)
{
	return std::format("{}({})",
					   toString(expr.valueType()),
					   formatFunctionArgumentList(expr.node()->inputs()));
}


std::string
CompilerGLSL::format(const CastExpression& expr)
{
	auto inputs = expr.node()->inputs();
	return std::format("({}){}",
					   toString(expr.valueType()),
					   resolve(*inputs.front()));
}


std::string
CompilerGLSL::format(const SwizzleExpression& expr)
{
	auto inputs = expr.node()->inputs();
	return std::format("{}.{}", 
		               resolve(*inputs.front()),
	                   toString(expr.swizzle()));
}


//std::string
//CompilerGLSL::format(const ConditionalExpression& expr)
//{
//	auto inputs = expr.inputs();
//	return std::format("({} ? {} : {})",
//					   resolve(*inputs[0]),
//					   resolve(*inputs[1]),
//					   resolve(*inputs[2]));
//}


std::string
CompilerGLSL::format(const SamplerExpression& expr)
{
	return expr.name();
}


std::string
CompilerGLSL::format(const Expression& expr)
{
	return std::visit(Visitor{ [this](const auto& e) { return format(e); }}, expr);
}


std::string
CompilerGLSL::resolve(const Node& node)
{
	auto iter = mNameLookUp.find(&node);
	if (iter != mNameLookUp.end())
	{
		return iter->second;
	}

	return format(node.expression());
}


std::string
CompilerGLSL::formatFunctionArgumentList(const std::vector<ConstNodePtr>& args)
{
	std::stringstream ss;

	for (auto [i, arg] : std::views::enumerate(args))
	{
		ss << resolve(*arg);

		if (i < args.size() - 1)
		{
			ss << ", ";
		}
	}

	return ss.str();
}


std::string
CompilerGLSL::buildUniformBlocksString()
{
	std::stringstream ss;

	std::unordered_map<const UniformBufferInfo*, std::vector<const UniformExpression*>> expressionsSorted;

	for (const auto& expression : mExpressions)
	{
		if (auto uniform = std::get_if<UniformExpression>(expression))
		{
			expressionsSorted[uniform->bufferInfo().get()].push_back(uniform);
		}
	}

	for (const auto& [buffer, members] : expressionsSorted)
	{
		ss << std::format("layout (std140, set = {}, binding = {}) uniform {}\n", 0, buffer->location(), buffer->name());
		ss << "{\n";

		for (const auto& member : members)
		{
			ss << TAB << toString(member->valueType()) << " " << member->name() << ";\n";
		}
		ss << "};\n";
	}

	if (!expressionsSorted.empty())
	{
		ss << "\n";
	}

	return ss.str();
}


std::string
CompilerGLSL::buildSamplerString()
{
	std::stringstream ss;

	std::set<const SamplerExpression*> samplers;

	for (auto expression : mExpressions)
	{
		if (auto sampler = std::get_if<SamplerExpression>(expression))
		{
			samplers.insert(sampler);
		}
	}

	for (auto sampler : samplers)
	{
		ss << std::format("layout(set = {}, binding = {}) uniform sampler2D {};\n", 0, sampler->location(), sampler->name());
	}

	if (!samplers.empty())
	{
		ss << "\n";
	}

	return ss.str();
}


std::string
CompilerGLSL::buildInputAttributeDefinitionsString()
{
	std::map<uint32_t, std::string> attributesSorted;
	for (const auto& attr : mShader->inputs())
	{
		auto location = attr->location();
		attributesSorted[location] = std::format("layout (location = {}) in {} {};\n",
			                                     location,
			                                     toString(attr->valueType()), format(*attr));
	}

	std::stringstream ss;
	for (auto [location, attr] : attributesSorted)
	{
		ss << attr;
	}

	if (!attributesSorted.empty())
	{
		ss << "\n";
	}

	return ss.str();
}


std::string
CompilerGLSL::buildOutputAttributeDefinitionsString()
{
	std::map<uint32_t, std::string> attributesSorted;
	for (const auto& attr : mShader->outputs())
	{
		std::visit(Visitor{
			[](DefaultSemantics)
			{
			}, 
			[&](const AttributeBinding& binding)
			{
				attributesSorted[binding.location] = std::format("layout (location = {}) out {} {};\n",
					                                             binding.location,
													             toString(attr->valueType()),
													             format(*attr));
			}
		}, attr->BindingInfo());
	}

	std::stringstream ss;
	for (auto [location, attr] : attributesSorted)
	{
		ss << attr;
	}

	if (!attributesSorted.empty())
	{
		ss << "\n";
	}

	return ss.str();
}


std::string
CompilerGLSL::buildMainFunctionString()
{
	std::stringstream ss;

	ss << "void main()\n" ;
	ss << "{\n";

	for (auto expr : mExpressions)
	{
		if (std::holds_alternative<InputAttributeExpression>(*expr)  ||
			std::holds_alternative<OutputAttributeExpression>(*expr) ||
			std::holds_alternative<UniformExpression>(*expr)         ||
			std::holds_alternative<SamplerExpression>(*expr))
		{
			continue;
		}

		auto node = std::visit(Visitor{ [](const auto& ex) { return ex.node(); } }, *expr);

		if (auto op = std::get_if<OperatorExpression>(expr))
		{
			if (op->getOperator() == Operator::ASSIGNMENT)
			{
				ss << TAB << format(*op) << ";\n";
			}
			continue;
		}

		if (!node->inlineIfPossible())
		{
			mNameLookUp[node.get()] = std::format("{}{}", getTypeShortName(node->valueType()), mVarCounter++);
			ss << TAB << std::format("{} {} = {};\n", toString(node->valueType()), mNameLookUp[node.get()], format(node->expression()));
			continue;
		}
	}

	auto outputs = mShader->outputs();
	for (const auto& output : outputs)
	{
		auto input = output->node()->inputs().front();
		if (input->inlineIfPossible())
		{
			ss << TAB << std::format("{} = {};\n", format(*output), format(input->expression()));
		}
		else
		{
			ss << TAB << std::format("{} = {};\n", format(*output), mNameLookUp[input.get()]);
		}
	}

	ss << "}\n";

	return ss.str();
}


std::expected<Compiler::Result, Compiler::Error>
CompilerGLSL::Compile(const ShaderGraph& shaderModule, ShaderStage stage)
{
	mShader = &shaderModule;
	
	mExpressions = mShader->expressions();

	Result result;

	auto inputAttributes  = buildInputAttributeDefinitionsString();
	auto outputAttributes = buildOutputAttributeDefinitionsString();
	auto uniforms	      = buildUniformBlocksString();
	auto samplers         = buildSamplerString();
	auto mainFunc	      = buildMainFunctionString();

	std::stringstream ss;
	ss << "#version 420\n\n";
	ss << inputAttributes;
	ss << outputAttributes;
	ss << uniforms;
	ss << samplers;
	ss << mainFunc;

	result.shaderCode = ss.str();

	return { std::move(result) };
}
