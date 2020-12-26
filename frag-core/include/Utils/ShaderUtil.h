/**
	FragEngine, A Two layer Game Engine.
    Copyright (C) 2018  Valdemar Lindberg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_SHADERUTIL_H_
#define _FRAG_CORE_SHADERUTIL_H_ 1
#include "../Core/IO/IO.h"
#include "../Core/Ref.h"
#include"../Renderer/IRenderer.h"
#include<vector>


namespace fragcore {
	
	/**
	 *
	 */
	class FVDECLSPEC ShaderUtil {	//TODO determine if to make a instance support for better memory improvements during compilation.
	public:
		typedef struct uniform_location_t {
			const char *texname; /*  */
			int loc;			 /*  */
		} UniformLocation;

		//private:
		typedef struct shader_object_t {
			const char *buf;
			unsigned long size;
			ShaderLanguage language;
			ShaderCodeType type;
		} ShaderObject;
		public:
			class ShaderObjectDesc
			{
				public:
				std::vector<Ref<IO>> source;
				ShaderLanguage language;
				ShaderCodeType type;
			};

		private:
			static void
			loadProgramFromMem(const ShaderObject *vshader, const ShaderObject *fshader, const ShaderObject *gshader,
							   const ShaderObject *tcshader,
							   const ShaderObject *teshader,
							   const ShaderObject *compute, Ref<IRenderer>& renderer, Shader **pshader);

		public:

		static void privateShaderPopulate();

		/*	Single shader object.	*/
		static void loadShader(Ref<IO> &io, ShaderType shadetype, Ref<IRenderer> &renderer, ShaderLanguage language, ShaderCodeType codetype, Shader **shader);
		static void loadShader(const char *source, const int size, ShaderType type, Ref<IRenderer> &renderer, ShaderLanguage language, ShaderCodeType codetype, Shader **pshader);
		static void loadShader(const ShaderObjectDesc *desc, ShaderType type, Ref<IRenderer> &renderer, ShaderLanguage language, ShaderCodeType codetype, Shader **pshader);



		static void loadProgram(Ref<IO> &io, Ref<IRenderer> &renderer, ShaderLanguage language, Shader **shader);
		static void loadProgram(Ref<IO> &vertex, Ref<IO> &fragment, Ref<IO> &geometry, Ref<IO> &tesselationC, Ref<IO> &tesselationF,
								ShaderCodeType codetype, ShaderLanguage lanaguage, Ref<IRenderer> &renderer, Shader **shader);
		static void loadProgram(const void *vertex, const int vsize,
								const void *fragment, const int fsize,
								const void *geometry, const int gsize,
								const void *tesselationc, const int tcsize,
								const void *tesselatione, const int tesize,
								ShaderCodeType codetype, ShaderLanguage lanaguage, Ref<IRenderer> &renderer, Shader **shader);

		static void loadProgram(const ShaderObjectDesc *vertex,
								const ShaderObjectDesc *fragment,
								const ShaderObjectDesc *geometry,
								const ShaderObjectDesc *tesseC,
								const ShaderObjectDesc *tesseT,
								const ShaderObjectDesc *compute,
								Ref<IRenderer> &renderer, Shader **shader);

		static void loadProgram(IO *io, IRenderer *renderer, Shader **pShader, unsigned int format);
		static void loadProgram(const void *pData, long int nBytes, IRenderer *renderer, Shader **pShader, unsigned int format);


		static void loadComputeProgram(Ref<IO> &computeIO, Ref<IRenderer> &renderer, ShaderLanguage language, ShaderCodeType codetype, Shader **program);
		static void loadComputeProgram(const char *pData, long int nBytes, ShaderLanguage language, ShaderCodeType codetype, Ref<IRenderer> &renderer, Shader **program);

		static void
		loadProgramPipeline(const ShaderObject *vshader, const ShaderObject *fshader, const ShaderObject *gshader,
							const ShaderObject *tcshader,
							const ShaderObject *teshader, IRenderer *renderer, ProgramPipeline **pshader);

		/*	*/
		static void
		loadComputeShaderSource(ShaderObject *shaderDesc, IRenderer *renderer, ProgramPipeline **programPipeline);

		static ShaderCodeType getCodeType(
			const char *filePath); /*  Determine if shader is binary or source file based on the file basename.    */
		static ShaderLanguage getFileLanguage(
			const char *filePath);							   /*  Determine what shader language based on the file basename.  */
		static ShaderType getShaderType(const char *filePath); /*  */

		static std::vector<UniformLocation> getShaderUniformAttributes(const Ref<Shader> &shader);

		//TODO resolve and act as a helper function.
		static void copyUniform(Ref<Shader> &shader);
		static void pastUniform(Ref<Shader> &shader);
	};
}

#endif