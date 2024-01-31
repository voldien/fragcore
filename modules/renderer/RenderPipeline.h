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
#ifndef _FRAG_CORE_PROGRAMPIPELINE_H_
#define _FRAG_CORE_PROGRAMPIPELINE_H_ 1
#include "RenderObject.h"

namespace fragcore {
	/**
	 *
	 */
	class RenderPipeline : public RenderObject {
		friend class IRenderer;

	  public:
		// TODO change to ShaderType
		enum ShaderType {
			VERTEX_SHADER = 0x1,				  /*	*/
			FRAGMENT_SHADER = 0x2,				  /*	*/
			GEOMETRY_SHADER = 0x4,				  /*	*/
			TESSELLATION_CONTROL_SHADER = 0x8,	  /*	*/
			TESSELLATION_EVOLUTION_SHADER = 0x10, /*	*/
			COMPUTE_SHADER = 0x20				  /*	*/
		};

		~RenderPipeline() override = default;

		/**
		 *
		 */
		virtual void bind() = 0;

		virtual int getLocation(const char *name) = 0;

		virtual void setInt(int location, int value) = 0;

		virtual void setFloat(int location, float value) = 0;

		virtual void setFloatv(int location, int n, const float *values) = 0;

		virtual void setVec2(int location, const float v1, const float v2) = 0;

		virtual void setVec2v(int location, int n, const float *values) = 0;

		virtual void setMatrix4f(int location, const float *pvalue) = 0;

		virtual void setMatrix4fv(int location, int n, const float *pvalue) = 0;

		virtual Shader *getShader(ShaderType type) = 0;

		virtual void setShader(ShaderType type, Shader *shader) = 0;
	};
} // namespace fragcore

#endif
