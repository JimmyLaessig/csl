///**
// * Copyright(c) 2026 Bernhard Rainer
// * SPDX-License-Identifier: MIT
// *
// * This file is part of C++ Shader Language (CSL) and is licensed under the MIT License.
// * See the LICENSE file in the project root for full license information.
// */
//
//#ifndef CSL_NODE_HPP
//#define CSL_NODE_HPP
//
//#include <csl/Expressions.hpp>
//
//namespace csl
//{
//
//class Node;
//
//// \brief Concept to ensure T is a std::shared_ptr<Node>
//template <typename T>
//concept NodePtrType = std::same_as<std::remove_cvref_t<T>, std::shared_ptr<Node>>;
//
//// \brief Class representing an node in the shader graph
//class CSL_API Node : public std::enable_shared_from_this<Node>
//{
//public:
//
//	Node(const Expression& expression, const std::vector<NodePtr>& inputs);
//
//	/** \brief Create a new node without inputs
//	 * \param expression The expression of the node
//	 * \return Returns a pointer to the created node.
//	 */
//	static NodePtr create(const Expression& expression);
//
//	/** \brief Create a new node with inputs
//	 * \param expression The expression of the node
//	 * \param inputs The nodes containing the inputs of the expression
//	 * \return Returns a pointer to the created node.
//	 */
//	static NodePtr create(const Expression& expression, const std::vector<NodePtr>& inputs);
//
//	/** \brief Create a new node with inputs
//	 * \param expression The expression of the node
//	 * \param inputs The nodes containing the inputs of the expression
//	 * \return Returns a pointer to the created node.
//	 */
//	template<NodePtrType ...Nodes>
//	static NodePtr create(const Expression& expression, Nodes&&... inputs)
//	{
//		return create(expression, std::vector<NodePtr>{ std::forward<Nodes>(inputs)... });
//	}
//
//	// \brief Get the expression's value type
//	ValueType valueType() const;
//
//	// \brief Get the expression
//	const Expression& expression() const;
//
//	/** \brief Set the expression
//	 * Replaces the current expression with the new one. The caller must ensure that current value types of the input
//	 * expressions match the required input value types of the new expression
//	 */
//	void setExpression(const Expression& expression);
//
//	/** \brief Get the list of inputs this expression
//	 * \return Returns a vector of pointers to this expression's inputs
//	 */
//	std::vector<std::shared_ptr<const Node>> inputs() const;
//
//	// \brief Flag indicating if the expression should be inlined during compilation
//	bool inlineIfPossible() const;
//
//	/** \brief Mark the node's expression to be inlined during compilation
//	 * Inlining an expression has the effect that the compiler tries to skip an explicit variable declaration to store
//	 * the expressions output. Instead, the compiler is allowed to construct the expression in-place.
//	 */
//	void setInlineIfPossible();
//
//private:
//
//	Expression mExpression;
//
//	std::vector<NodePtr> mInputs;
//
//	bool mInline{ false };
//
//}; // class Node
//
//} // namespace csl
//
//#endif // !CSL_NODE_HPP
