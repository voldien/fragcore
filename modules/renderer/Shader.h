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
#ifndef _FRAGCORE_SHADER_H_
#define _FRAGCORE_SHADER_H_ 1
#include "RenderObject.h"

namespace fragcore {
	/**
	 * Shader.
	 */
	class FVDECLSPEC Shader : public RenderObject {
		friend class IRenderer;

	  public:
		~Shader() override = default;

		virtual void bind() = 0;

		virtual int getLocation(const char *cparamname) = 0;

		virtual void setInt(int location, int value) = 0;
		// virtual void setIntv(int location, int n, const float *values);

		virtual void setFloat(int location, float value) = 0;

		virtual void setFloatv(int location, int n, const float *values) = 0;

		virtual void setVec2(int location, const float v1, const float v2) = 0;

		virtual void setVec2v(int location, int n, const float *values) = 0;

		virtual void setMatrix4f(int location, const float *pvalue) = 0;

		virtual void setMatrix4fv(int location, int n, const float *pvalue) = 0;

		// virtual int getNumberUniform();
		// virtual void getUniform(int location);

		//	virtual void getAttributeLocatoin(const char* cparamname);
		//	virtual void getFragmentLocation(const char* cparamname);

		//	virtual void setMatrix3f(int location, const float *pvalue);
		//
		//	virtual void setMatrix3fv(int location, int n, const float *pvalue);

		//	enum ShaderPrecision{
		//	    eUnknown,
		//	    eLow,
		//	    eMedium,
		//	    eFull,
		//	};
		//	virtual ShaderPrecision getShaderPrecision();

		// virtual void bindBuffer(const char*  name,
		//                       Buffer * buffer);

		// virtual void bindUniformBuffer(const char* name, Buffer* buffer);

		// virtual void* mapBindingUniform(int location);
		// virtual void unMapBindingUniform(int location);

		//	virtual void setFragLocation(const char* name, int index);
		//	virtual void setAttributeLocation(const char* name, int index);

		virtual void getLocalGroupSize(int *sizes) = 0;

		// TODO determine if relocate or not!
		virtual void *getBinary(long int *size, unsigned int *format) = 0;

		// virtual void* getBinaryShader(long int* size, unsigned int* fvformatf, int p);
		virtual void *getSource(long int *size) = 0; // TODO rename to the shader version instead of program.
	};
} // namespace fragcore

#endif
