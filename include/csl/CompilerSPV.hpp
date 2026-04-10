/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_COMPILERSPV_HPP
#define CSL_COMPILERSPV_HPP

#include <csl/CompilerGLSL.hpp>

namespace csl
{

class CSL_API CompilerSPV : public Compiler
{
public:

	std::expected<Result, Error> Compile(const ShaderGraph& shaderModule, ShaderStage stage) override;

	Result GetShaderSourceGLSL() const;

private:

	std::expected<Result, Error> CompileSPV(const Result& shaderSourceGLSL, ShaderStage stage);

	CompilerGLSL mCompilerGLSL;

	const ShaderGraph* mShaderModule{ nullptr };

	Result mShaderSourceGLSL;
};

} // namespace csl

#endif // !CSL_COMPILERSPV_HPP
