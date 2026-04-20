/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_COMPILERGLSL_HPP
#define CSL_COMPILERGLSL_HPP

#include <csl/Compiler.hpp>

#include <csl/Expressions.hpp>

#include <unordered_map>

namespace csl
{

class CSL_API CompilerGLSL : public Compiler
{
public:

	std::expected<Result, Error> Compile(const ShaderGraph& shader, ShaderStage stage) override;

private:

	std::string format(const ConstantExpression<float>& expr);

	std::string format(const ConstantExpression<int>& expr);

	std::string format(const ConstantExpression<bool>& expr);

	std::string format(const InputAttributeExpression& expr);

	std::string format(const OutputAttributeExpression& expr);

	std::string format(const OperatorExpression& expr);

	std::string format(const UniformBufferExpression& expr);

	std::string format(const UniformExpression& expr);

	std::string format(const NativeFunctionExpression& expr);

	std::string format(const ConstructorExpression& expr);

	std::string format(const CastExpression& expr);

	std::string format(const SwizzleExpression& expr);

	std::string format(const SamplerExpression& expr);

	std::string format(const IfExpression& expr);

	std::string format(const ElseIfExpression& expr);

	std::string format(const ElseExpression& expr);

	std::string format(const EndIfExpression& expr);

	std::string formatExpression(const Expression& expr);

	std::string resolve(const Expression& expr);

	std::string formatFunctionArgumentList(const std::vector<std::shared_ptr<const Expression>>& expressions);

	std::string buildInputAttributeDefinitionsString();

	std::string buildOutputAttributeDefinitionsString();

	std::string buildMainFunctionString();

	std::string buildUniformBlocksString();

	std::string buildSamplerString();

	std::vector<const Expression*> mExpressions;

	const ShaderGraph* mShader{ nullptr };

	std::unordered_map<const Expression*, std::string> mNameLookUp;

	uint32_t mDefaultDescriptorSet{ 0 };

	uint32_t mVarCounter{ 0 };
};

} // namespace csl

#endif // !CSL_COMPILERGLSL_HPP
