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
#ifndef _FRAG_CORE_RENDERING_FACTORY_H_
#define _FRAG_CORE_RENDERING_FACTORY_H_ 1
#include "IRenderer.h"

namespace fragcore {
	/**
	 *	Responsible for creating a rendering
	 *	interface.
	 */
	class FVDECLSPEC RenderingFactory {
	  public:
		enum RenderingAPI {
			OpenGL = 0,	 /*	OpenGL rendering API.	*/
			Vulkan = 1,	 /*	Vulkan rendering API.	*/
			DirectX = 2, /*	DirectX rendering API.	*/
			OpenCL = 3,
			/*	OpenCL rendering API.	*/ // TODO remove since it will be relocated to the computeinterface class.
		};

		/**
		 * @brief Create Renderer.
		 * @param renderingapi rendering API.
		 * @return none NULL if successful.
		 */
		static IRenderer *createRendering(RenderingAPI renderingapi, IConfig *config);

		/**
		 * @brief Create Renderer.
		 * @param cpathlib valid library path name or library name.
		 * @return none NULL if successful.
		 */
		static IRenderer *createRendering(const char *cpathlib, IConfig *config);

		/**
		 * @brief Get rendering interface library path.
		 * @param api valid Rendering API.
		 * @return non-null terminated string.
		 */
		static const char *getInterfaceLibraryPath(RenderingAPI api);

	  private: /*	Prevent one from creating an instance of this class.	*/
		RenderingFactory() = delete;
		RenderingFactory(const RenderingFactory &other) = delete;
	};
} // namespace fragcore

#endif
