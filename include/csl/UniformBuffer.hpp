#ifndef CSL_UNIFORMBUFFER_HPP
#define CSL_UNIFORMBUFFER_HPP

#include <csl/Binding.hpp>
#include <csl/Expressions.hpp>
#include <csl/ShaderGraph.hpp>
#include <csl/Node.hpp>

namespace csl
{

class CSL_API UniformBufferBase
{
public:

	template<StringLiteral Name, typename T>
	void RegisterMember(T& member)
	{
		member.node()->setExpression(UniformExpression(T::ValueType, Name, mBufferInfo));
	}

protected:

	UniformBufferBase(uint32_t location, std::string_view name)
		: mBufferInfo(std::make_shared<UniformBufferInfo>(location, name))
	{
	}

private:

	std::shared_ptr<UniformBufferInfo> mBufferInfo;
};


template<typename T, StringLiteral Name, Location L>
class CSL_API UniformBuffer : protected UniformBufferBase
{
public:

	UniformBuffer()
		: UniformBufferBase(static_cast<uint32_t>(L), Name)
	{
		DefineType(mValue, static_cast<UniformBufferBase&>(*this));
	}

	const T* operator->() const
	{
		return &mValue;
	}

	const T& operator*() const
	{
		return mValue;
	}

	operator T() const
	{
		return mValue;
	}

private:

	T mValue;
};

} // namespace csl 

#endif // !CSL_UNIFORMBUFFER_HPP
