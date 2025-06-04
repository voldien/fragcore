/*
 *	FragCore - Fragment Core - Engine Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FRAGCORE_SHADER_COMPILER_H_
#define _FRAGCORE_SHADER_COMPILER_H_ 1
#include "../Shader.h"
#include "ShaderLanguage.h"
#include <FragDef.h>
#include <map>
#include <vector>

namespace fragcore {

	/**
	 * @brief
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
		using CompilerConvertOption = struct compiler_convert_option_t {
			ShaderLanguage target = ShaderLanguage::GLSL;
			unsigned int glslVersion = 150;
			bool es = false;
			bool auto_storage_qualifier = false;
			unsigned int precision = 0;
		};

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
		static std::vector<char> convertSPIRV(const std::vector<uint32_t> &source, const CompilerConvertOption &target);
	};
} // namespace fragcore
#endif
