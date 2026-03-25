#ifndef CSL_SHADERGRAPH_HPP
#define CSL_SHADERGRAPH_HPP

#include <csl/Expressions.hpp>
#include <memory>
#include <vector>
#include <cassert>

namespace csl
{

class Node;

// ============================================================
//  FunctionTraits — handles function pointers
// ============================================================

template <typename T>
struct FunctionTraits;

template <typename C, typename R, typename... Args>
struct FunctionTraits<R(C::*)(Args...)>
{
	using ClassType  = C;
	using ReturnType = R;
	using ArgsTuple  = std::tuple<std::remove_cvref_t<Args>...>;
};


class CSL_API ShaderGraph
{
public:

	template<typename T>
	ShaderGraph(T&& shader)
	{
		auto main = &T::main;

		using Args = FunctionTraits<decltype(main)>::ArgsTuple;

		sCurrentShaderModule = this;
		{
			Args args{};

			auto result = std::apply(
				[&](auto&&... unpacked) {
					return (shader.*main)(std::forward<decltype(unpacked)>(unpacked)...);
				},
				args
			);
		}

		sCurrentShaderModule = nullptr;
	}

	std::vector<const InputAttributeExpression*> inputs() const;

	std::vector<const OutputAttributeExpression*> outputs() const;

	std::vector<const UniformExpression*> uniforms() const;

	std::vector<std::shared_ptr<const Node>> expressions() const;

	void addNode(std::shared_ptr<Node> node);

	static ShaderGraph* current() { return sCurrentShaderModule; }

private:

	static inline ShaderGraph* sCurrentShaderModule{ nullptr };

	std::vector<NodePtr> mNodes;

}; // class ShaderModule

} // namespace csl 

#endif // !CSL_SHADERGRAPH_HPP
