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
#ifndef _FRAGCORE_VK_COMPUTE_H_
#define _FRAGCORE_VK_COMPUTE_H_ 1
#include "../ICompute.h"
#include "../RenderPrerequisites.h"
#include <Core/Module.h>
#include <Core/SmartReference.h>
#include <FragDef.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC VKCompute : public ICompute {
	  public:
		void onInitialization() override;
		void onDestruction() override;

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		bool supportCompute() noexcept override;

		const char *getVersion() const override;

		VKCompute() = default;
		VKCompute(const VKCompute &other) = delete;
		VKCompute(VKCompute &&other) = delete;
		~VKCompute() override = default;
	};
} // namespace fragcore
#endif
