/*
 *	FragCore - Fragment Core
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
#include "Core/UIDStruct.h"
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
		enum class CompilerAttributeOptionType {
			Undefined,
			Macro,
			Constant,
			MaxOptionAttributeTypes,
		};

		class CompilerOption {
		  public:
			CompilerAttributeOptionType type;
			const char *name;
			union {
				const char *value;
				const int intValue;
				const float floatValue;
				const double doubleValue;
			};
		};

		class ShaderResult : UIDObject {
		  public:
			Ref<Shader> shader;
			CompilerOption option;
		};

		class CompilerOptionSet {
		  public:
			std::vector<CompilerOption> options;
		};

		using CompilerConvertOption = struct compiler_convert_option_t {
			ShaderLanguage target = ShaderLanguage::GLSL;
			unsigned int glslVersion = 150;
			bool es = false;
			bool auto_storage_qualifier = false;
			unsigned int precision = 0; /*	*/
		};

		using ConversionResult = struct compilation_result_t {
			std::vector<char> source;
			// options
		};

		/**
		 * Convert spirv binary to source code. (decompilition)
		 */
		static std::vector<char> convertSPIRV(const std::vector<uint32_t> &sourceBinary,
											  const CompilerConvertOption &targetOptions);

		/**
		 * Convert spirv binary to source code. (decompilition)
		 */
		static std::vector<ConversionResult> convertSPIRVPermutation(const std::vector<uint32_t> &sourceBinary,
																	 const CompilerConvertOption &targetOptions,
																	 const CompilerOptionSet &set);
	};
} // namespace fragcore
#endif
