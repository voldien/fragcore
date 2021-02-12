#include"Utils/ShaderCompiler.h"
#include"Utils/ShaderUtil.h"
#include"Core/dataStructure/StackAllactor.h"
#include"Core/IO/BufferIO.h"
#include"Utils/StringUtil.h"

using namespace fragcore;

std::map<long int, ShaderCompiler::ShaderResult>
ShaderCompiler::CompilePermutation(Ref<IRenderer> &renderer, CompilerSources *references, const CompilerOptionSet &optionset)
{
	std::vector<CompilerOption>::const_iterator it = optionset.option.cbegin();

	std::map<long int, ShaderCompiler::ShaderResult> result;

	StackAllocator alloctor(128000);

	size_t cbuffer = 4096;
	it = optionset.option.cbegin();
	for (; it != optionset.option.cend(); it++)
	{

		/*	Write whole macro definition for all the options.	*/
		void *string_options = alloctor.allocateAligned(cbuffer, 8);
		BufferIO bufferIO(string_options, cbuffer);

		std::vector<CompilerOption>::const_iterator eit = optionset.option.cbegin();
		for (; eit != optionset.option.cend(); eit++)
		{
			
			std::string macro = fvformatf("#define %s %s", (*eit).name, (*eit).value);
			bufferIO.write(macro.size() + 1, macro.c_str());
		}

		/*	Load source and combine.	*/

		//ShaderUtil::ShaderObjectDesc _v = {};
//		ShaderUtil::ShaderObjectDesc _g, _f, _tc, _te, _c;

		long int resultID = 0;
		Shader *shader;
		ShaderUtil::loadProgram(references->vertex,
								references->fragment,
								references->geometry,
								references->tesseC,
								references->tesseT,
								references->compute,
								renderer,
								&shader);
		result[resultID].shader = shader;
		//result[resultID].option
		alloctor.clear();
	}

	return result;
}

#include <spirv_cross/spirv_cross_c.h>

void CompileCrossShader(Ref<IO> &io, Ref<IO> &out){

	// const SpvId *spirv = NULL;
	// //get_spirv_data();
	// size_t word_count = 0;
	// //get_spirv_word_count();

	// spvc_context context = NULL;
	// spvc_parsed_ir ir = NULL;
	// spvc_compiler compiler_glsl = NULL;
	// spvc_compiler_options options = NULL;
	// spvc_resources resources = NULL;
	// const spvc_reflected_resource *list = NULL;
	// const char *result = NULL;
	// size_t count;
	// size_t i;

	// // Create context.
	// spvc_context_create(&context);

	// // Set debug callback.
	// spvc_context_set_error_callback(context, NULL, NULL);

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