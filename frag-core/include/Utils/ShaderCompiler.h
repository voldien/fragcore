#ifndef _FRAG_CORE_SHADER_COMPILER_H_
#define _FRAG_CORE_SHADER_COMPILER_H_ 1
#include"../Def.h"
#include"../Renderer/Shader.h"
#include"../Core/Ref.h"
#include"ShaderUtil.h"
#include<map>
#include<vector>

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC ShaderCompiler{
	public:
		class CompilerOption : UIDObject {
			public:
			unsigned int type;
			const char *name;
			const char *value;
		};

		class CompilerSources {
		public:
			const ShaderUtil::ShaderObjectDesc *vertex;
			const ShaderUtil::ShaderObjectDesc *fragment;
			const ShaderUtil::ShaderObjectDesc *geometry;
			const ShaderUtil::ShaderObjectDesc *tesseC;
			const ShaderUtil::ShaderObjectDesc *tesseT;
			const ShaderUtil::ShaderObjectDesc *compute;

			// Ref<IO> vertex;
			// Ref<IO> fragment;
			// Ref<IO> geometry;
			// Ref<IO> tessellationControl;
			// Ref<IO> tessellationEvolution;
			// Ref<IO> compute;
		};

		class ShaderResult : UIDObject {
		public:
			Ref<Shader> shader;
			CompilerOption option;
		};

		class CompilerOptionSet {
			public:
			std::vector<CompilerOption> option;
		};

		std::map<long int, ShaderResult>
		CompilePermutation(Ref<IRenderer> &renderer, CompilerSources* references, const CompilerOptionSet &optionset);

		/**/
		void CompileCrossShader(Ref<IO> &io, Ref<IO>& out);
  	};
}
#endif
