#include "ShaderCompiler.h"
#include "ShaderLanguage.h"
#include <spirv_glsl.hpp>
#include <spirv_hlsl.hpp>
#include <vector>
// #include <spirv_cross/spirv_msl.hpp>

#include <fmt/core.h>
using namespace fragcore;

std::vector<char> ShaderCompiler::convertSPIRV(const std::vector<uint32_t> &sourceBinary,
											   const CompilerConvertOption &targetOptions) {

	switch (targetOptions.target) {
	case ShaderLanguage::GLSL:
		break;
	case ShaderLanguage::HLSL:
		// spirv_cross::CompilerHLSL(reinterpret_cast<const uint32_t *>(sourceCode.data()), sourceCode.size() /
		// sizeof(uint32_t))
		break;
	case SPIRV:
	case CLC:
	case CG:
	case unKnownLanguage:
		break;
	default:
		break;
	}

	/*	*/
	if (targetOptions.target == ShaderLanguage::GLSL) {
		spirv_cross::CompilerGLSL glsl(sourceBinary);

		// Set some options.
		spirv_cross::CompilerGLSL::Options options; // = glsl.get_common_options();
		options.version = targetOptions.glslVersion;
		options.es = false;
		options.enable_storage_image_qualifier_deduction = false;
		options.fragment.default_float_precision = spirv_cross::CompilerGLSL::Options::Precision::DontCare;

		glsl.set_common_options(options);

		// Compile to GLSL, ready to give to GL driver.
		const std::string converted_source = glsl.compile();

		/*	Includes the null terminator.	*/
		return std::vector<char>(converted_source.begin(), converted_source.end() + 1);
	}

	return {};
}