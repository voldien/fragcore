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
#ifndef _FRAGCORE_RENDEROBJECT_H_
#define _FRAGCORE_RENDEROBJECT_H_ 1
#include "RenderPrerequisites.h"
#include <Core/SmartReference.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC RenderObject : public SmartReference {
		friend class IRenderer;

	  public:
		RenderObject() = default;
		RenderObject(IRenderer *renderer) { setRenderInterface(renderer); }
		RenderObject(RenderObject &&other) = delete;
		~RenderObject() override = default;

		/**
		 * @brief Get the Renderer object
		 *
		 * @return T*
		 */
		template <class T = IRenderer> inline T *getRenderer() const noexcept {
			return static_cast<T *>(this->iRenderer);
		}

		/**
		 * @brief Get the Object object
		 *
		 * @return void*
		 */
		inline void *getObject() noexcept { return this->pdata; }

		/**
		 * @brief Get the Object object
		 *
		 * @return void*
		 */
		inline void *getObject() const noexcept { return this->pdata; }

		/**
		 * @brief Get the Native Ptr object
		 *
		 * @return intptr_t
		 */
		virtual intptr_t getNativePtr() const = 0;

		/**
		 * @brief Set the Render Interface object
		 *
		 * @param interface
		 */
		virtual void setRenderInterface(IRenderer *interface) noexcept { this->iRenderer = interface; }

	  protected: /*  */
		void *pdata;
		// Ref<IRenderer> irenderer;
		IRenderer *iRenderer; // TODO change to ref.
	};
} // namespace fragcore

#endif
