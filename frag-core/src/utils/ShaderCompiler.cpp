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