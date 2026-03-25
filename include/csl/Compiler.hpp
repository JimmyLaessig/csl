#ifndef CSL_COMPILER_HPP
#define CSL_COMPILER_HPP

#include <csl/Export.hpp>

#include <csl/ShaderGraph.hpp>

#include <expected>
#include <map>
#include <string_view>

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
