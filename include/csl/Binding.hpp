#ifndef CSL_BINDING_HPP
#define CSL_BINDING_HPP

#include <string_view>

namespace csl
{

template <size_t N>
struct StringLiteral
{ 
	constexpr StringLiteral(const char(&str)[N])
	{ 
		for (size_t i = 0; i < N; ++i)
		{
			value[i] = str[i];
		}
	}

	constexpr operator std::string_view() const
	{ 
		return { value, N - 1 };
	}

	char value[N];
};

enum class Location : uint32_t {};

} // namespace csl

#endif // !CSL_BINDING_HPP
