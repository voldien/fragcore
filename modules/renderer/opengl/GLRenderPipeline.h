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
#ifndef _FRAG_CORE_GL_PROGRAMPIPELINE_H_
#define _FRAG_CORE_GL_PROGRAMPIPELINE_H_ 1
#include "../RenderPipeline.h"
//#include "GLShader.h"

namespace fragcore {
	/**
	 *
	 */
	class GLRenderPipeline : public RenderPipeline {
		friend class IRenderer;

	  public:
		virtual ~GLRenderPipeline() = default;

		/**
		 *
		 */
		virtual void bind() override;

		virtual int getLocation(const char *name) override;

		virtual void setInt(int location, int value) override;

		virtual void setFloat(int location, float value) override;

		virtual void setFloatv(int location, int n, const float *values) override;

		virtual void setVec2(int location, const float v1, const float v2) override;

		virtual void setVec2v(int location, int n, const float *values) override;

		virtual void setMatrix4f(int location, const float *pvalue) override;

		virtual void setMatrix4fv(int location, int n, const float *pvalue) override;

		virtual Shader *getShader(ShaderType type) override;

		virtual void setShader(ShaderType type, Shader *shader) override;

		intptr_t getNativePtr() const override;

	  public:
		unsigned int getProgram() const noexcept { return this->program; }

	  private:
		unsigned int program;
		// GLShader *v, *f, *g, *tc, *te, *c;
	};
} // namespace fragcore

#endif
