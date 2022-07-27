#include "ShaderCompiler.h"
#include "Core/IO/BufferIO.h"
#include "Core/dataStructure/StackAllactor.h"
#include "ShaderUtil.h"
#include <spirv_cross/spirv_glsl.hpp>
#include <spirv_cross/spirv_hlsl.hpp>
#include <spirv_cross/spirv_msl.hpp>

#include <fmt/core.h>
using namespace fragcore;

std::vector<char> ShaderCompiler::convert(const std::vector<char> &sourceCode, ShaderLanguage source,
										  ShaderLanguage target) {
	spirv_cross::Compiler *compiler = nullptr;
	// spirv_cross::CompilerHLSL(reinterpret_cast<const uint32_t *>(sourceCode.data()), sourceCode.size() /
	// sizeof(uint32_t))

	return {};
}

std::vector<char> ShaderCompiler::convertSPIRV(const std::vector<char> &source, ShaderLanguage shaderLanguage) {
	// Read SPIR-V from disk or similar.

	spirv_cross::CompilerGLSL glsl(reinterpret_cast<const uint32_t *>(source.data()), source.size() / sizeof(uint32_t));

	// The SPIR-V is now parsed, and we can perform reflection on it.
	spirv_cross::ShaderResources resources = glsl.get_shader_resources();

	// Get all sampled images in the shader.
	for (auto &resource : resources.sampled_images) {
		unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
		unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
		printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);

		// Modify the decoration to prepare it for GLSL.
		glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);

		// Some arbitrary remapping if we want.
		glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);
	}

	// Set some options.
	spirv_cross::CompilerGLSL::Options options;
	options.version = 330;
	options.es = false;
	glsl.set_common_options(options);

	// Compile to GLSL, ready to give to GL driver.

	std::string converted_source = glsl.compile();
	/*	*/
	return std::vector<char>(converted_source.begin(), converted_source.end());
}

// std::map<long int, ShaderCompiler::ShaderResult>
// ShaderCompiler::CompilePermutation(Ref<IRenderer> &renderer, CompilerSources *references,
// 								   const CompilerOptionSet &optionset) {
// 	std::vector<CompilerOption>::const_iterator it = optionset.option.cbegin();

// 	std::map<long int, ShaderCompiler::ShaderResult> result;

// 	StackAllocator alloctor(128000);

// 	size_t cbuffer = 4096;
// 	it = optionset.option.cbegin();
// 	for (; it != optionset.option.cend(); it++) {

// 		/*	Write whole macro definition for all the options.	*/
// 		void *string_options = alloctor.allocateAligned(cbuffer, 8);
// 		BufferIO bufferIO(string_options, cbuffer);

// 		std::vector<CompilerOption>::const_iterator eit = optionset.option.cbegin();
// 		for (; eit != optionset.option.cend(); eit++) {

// 			std::string macro = fmt::format("#define %s %s", (*eit).name, (*eit).value);
// 			bufferIO.write(macro.size() + 1, macro.c_str());
// 		}

// 		/*	Load source and combine.	*/

// 		// ShaderUtil::ShaderObjectDesc _v = {};
// 		//		ShaderUtil::ShaderObjectDesc _g, _f, _tc, _te, _c;

// 		long int resultID = 0;
// 		Shader *shader;
// 		ShaderUtil::loadProgram(references->vertex, references->fragment, references->geometry, references->tesseC,
// 								references->tesseT, references->compute, renderer, &shader);
// 		result[resultID].shader = shader;
// 		// result[resultID].option
// 		alloctor.clear();
// 	}

// 	return result;
// }

//#include <spirv_cross/spirv_cross_c.h>

void CompileCrossShader(Ref<IO> &io, Ref<IO> &out) {

	// const SpvId *spirv = nullptr;
	// //get_spirv_data();
	// size_t word_count = 0;
	// //get_spirv_word_count();

	// spvc_context context = nullptr;
	// spvc_parsed_ir ir = nullptr;
	// spvc_compiler compiler_glsl = nullptr;
	// spvc_compiler_options options = nullptr;
	// spvc_resources resources = nullptr;
	// const spvc_reflected_resource *list = nullptr;
	// const char *result = nullptr;
	// size_t count;
	// size_t i;

	// // Create context.
	// spvc_context_create(&context);

	// // Set debug callback.
	// spvc_context_set_error_callback(context, nullptr, nullptr);

	// // Parse the SPIR-V.
	// spvc_context_parse_spirv(context, spirv, word_count, &ir);

	// // Hand it off to a compiler instance and give it ownership of the IR.
	// spvc_context_create_compiler(
	// 	context, SPVC_BACKEND_GLSL, ir, SPVC_CAPTURE_MODE_TAKE_OWNERSHIP, &compiler_glsl);

	// // Do some basic reflection.
	// spvc_compiler_create_shader_resources(compiler_glsl, &resources);
	// spvc_resources_get_resource_list_for_type(
	// 	resources, SPVC_RESOURCE_TYPE_UNIFORM_BUFFER, &list, &count);

	// for (i = 0; i < count; i++) {
	// 	printf("ID: %u, BaseTypeID: %u, TypeID: %u, Name: %s\n",
	// 		   list[i].id,
	// 		   list[i].base_type_id,
	// 		   list[i].type_id,
	// 		   list[i].name);
	// 	printf("  Set: %u, Binding: %u\n",
	// 		   spvc_compiler_get_decoration(compiler_glsl, list[i].id, SpvDecorationDescriptorSet),
	// 		   spvc_compiler_get_decoration(compiler_glsl, list[i].id, SpvDecorationBinding));
	// }

	// // Modify options.
	// spvc_compiler_create_compiler_options(context, &options);
	// spvc_compiler_options_set_uint(options, SPVC_COMPILER_OPTION_GLSL_VERSION, 330);
	// spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_ES, SPVC_FALSE);
	// spvc_compiler_install_compiler_options(compiler_glsl, options);

	// spvc_compiler_compile(compiler, &result);
	// printf("Cross-compiled source: %s\n", result);

	// // Frees all memory we allocated so far.
	// spvc_context_destroy(context);
}