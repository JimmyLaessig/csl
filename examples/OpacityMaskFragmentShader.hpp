#ifndef CSL_EXAMPLES_OPACITYMASKFRAGMENTSHADER_HPP
#define CSL_EXAMPLES_OPACITYMASKFRAGMENTSHADER_HPP

#include <csl/csl.hpp>

#include "SimpleVertexShader.hpp"
#include "DiffuseTextureFragmentShader.hpp"

namespace csl::Examples
{

struct OpacityMaskFragmentShader : DiffuseTextureFragmentShader
{
	csl::Sampler2D opacityTexture{ 3, "OpacityMask" };

	FragmentOutput main(const FragmentInput& input)
	{
		auto mask = opacityTexture.sample(input.texcoord0).w();
		csl::If(mask < 1.f, []
		{
			csl::discard();
		});

		return DiffuseTextureFragmentShader::main(input);
	}
};

} // namespace csl::Examples

#endif // !CSL_EXAMPLES_OPACITYMASKFRAGMENTSHADER_HPP