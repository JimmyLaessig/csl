/**
 * Copyright(c) 2026 Bernhard Rainer
 * SPDX-License-Identifier: MIT
 *
 * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#ifndef CSL_SHADERGRAPH_HPP
#define CSL_SHADERGRAPH_HPP

#include <csl/Export.hpp>

#include <csl/Attribute.hpp>

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace csl
{

class Node;
class InputAttributeExpression;
class OutputAttributeExpression;
class UniformExpression;

// \brief Concept to detect struct-like types
template <typename T>
concept StructType = std::is_class_v<T> && !std::is_union_v<T>;

// \brief Minimal FunctionTraits for member functions
template <typename>
struct FunctionTraits;

// \brief Member function pointer traits
template <typename C, typename R, typename Arg>
struct FunctionTraits<R(C::*)(Arg)>
{
	using ClassType  = C;
	using ReturnType = R;
	using ArgType    = Arg;
};

// \brief Const member function pointer traits
template <typename C, typename R, typename Arg>
struct FunctionTraits<R(C::*)(Arg) const>
{
	using ClassType  = C;
	using ReturnType = R;
	using ArgType    = Arg;
};

// \brief Concept for Shader structs
template <typename T>
concept ShaderWithMainFunc = requires
	// Ensure T::main exists
	{ { &T::main } -> std::same_as<decltype(&T::main)>; }
	&&
	// Ensure that main takes one struct and returns one struct
	[] {
		using Traits = FunctionTraits<decltype(&T::main)>;
		using Arg    = typename Traits::ArgType;
		using Ret    = typename Traits::ReturnType;

		return StructType<std::remove_cvref_t<Arg>> &&
			   StructType<Ret>;
	}();


/** \brief Class describing a shader graph
 * 
 */
class CSL_API ShaderGraph
{
public:

	/** \brief Construct a ShaderGraph from a Shader object 
	 * \param shader The shader object. The shader object must adhere to the ShaderWithMainFunc concept.
	 */
	template<typename T>
	ShaderGraph(T& shader)
	{
		using Traits = FunctionTraits<decltype(&T::main)>;
		using Arg    = std::remove_cvref_t<typename Traits::ArgType>;
		using Ret    = typename Traits::ReturnType;
		beginRecording();

		csl::Attributes inputAttributeBuilder(true);
		csl::Attributes outputAttributeBuilder(false);

		Arg arg;
		DefineType(arg, inputAttributeBuilder);
		Ret result = shader.main(arg);
		DefineType(result, outputAttributeBuilder);
		endRecording();
	}

	/** \brief Get all input attribute expressions in the shader graph
	 * \return Vector containing pointers to all input attribute expressions of the shader graph.
	 */
	std::vector<const InputAttributeExpression*> inputs() const;

	/** \brief Get all output attribute expressions in the shader graph
	 * \return Vector containing pointers to all output attribute expressions of the shader graph.
	 */
	std::vector<const OutputAttributeExpression*> outputs() const;

	/** \brief Get all output uniform expressions in the shader graph
	 * \return Vector containing pointers to all uniform attribute expressions of the shader graph.
	 */
	std::vector<const UniformExpression*> uniforms() const;

	/** \brief Get a list of all expressions in the shader graph 
	 * \return The expressions of the shader sorted by order of execution
	 */
	std::vector<const Expression*> expressions() const;

	/** \brief Add the node to the shader graph
	 * This function is used by the Node's create() function to add itself to the shader graph
	 */
	void addNode(std::shared_ptr<Node> node);

	/** \brief Begin recording of shader graph expressions
	 */
	void beginRecording();

	/** \brief End recording of shader graph expressions
     */
	void endRecording();

	/** \brief Get the current recording shader graph
	 * This function is used by the Node's create() function to add itself to the shader graph
     */
	static ShaderGraph* current();

private:

	std::vector<std::shared_ptr<Node>> mNodes;

}; // class ShaderGraph

} // namespace csl 

#endif // !CSL_SHADERGRAPH_HPP
