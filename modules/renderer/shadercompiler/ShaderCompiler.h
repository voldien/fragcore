/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAG_CORE_SHADER_COMPILER_H_
#define _FRAG_CORE_SHADER_COMPILER_H_ 1
#include "../Shader.h"
#include "ShaderUtil.h"
#include <Core/Ref.h>
#include <FragDef.h>
#include <map>
#include <string>
#include <vector>

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC ShaderCompiler {
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
		static std::map<long int, ShaderResult>
		CompilePermutation(Ref<IRenderer> &renderer, CompilerSources *references, const CompilerOptionSet &optionset);

	  public:
		/**
		 * @brief
		 *
		 * @param sourceCode
		 * @param source
		 * @param target
		 * @return std::vector<char>
		 */
		static std::vector<char> convert(const std::vector<char> &sourceCode, ShaderLanguage source,
										 ShaderLanguage target);

		/**
		 * @brief
		 *
		 * @param source
		 * @param shaderLanguage
		 * @return std::vector<char>
		 */
		static std::vector<char> convertSPIRV(const std::vector<char> &source, ShaderLanguage shaderLanguage);

		/**/
		// void CompileCrossShader(Ref<IO> &io, Ref<IO> &out);
	};
} // namespace fragcore
#endif
