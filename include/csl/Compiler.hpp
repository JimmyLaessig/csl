/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_COMPILER_HPP
#define CSL_COMPILER_HPP

#include <csl/Export.hpp>

#include <csl/ShaderGraph.hpp>

#include <expected>
#include <string>

namespace csl
{

enum class ShaderStage
{
	VERTEX,
	FRAGMENT
};

class CSL_API Compiler
{
public:

	struct Result
	{
		std::string shaderCode;
	};

	struct Error
	{
		std::string message;
	};

	/// Compile the shader program
	virtual std::expected<Result, Error> Compile(const ShaderGraph& shaderGraph, ShaderStage stage) = 0;
};

} // namespace Coral

#endif // !CSL_COMPILER_HPP
