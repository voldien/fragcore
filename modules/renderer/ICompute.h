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
#ifndef _FRAGCORE_ICOMPUTE_H_
#define _FRAGCORE_ICOMPUTE_H_ 1
#include <Core/Module.h>
#include <Core/SmartReference.h>
#include <FragDef.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC ICompute : public Module {
	  public:
		void onInitialization() override = 0;
		void onDestruction() override = 0;

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		virtual bool supportCompute() noexcept = 0;

		virtual const char *getVersion() const = 0;

		ICompute() = default;
		ICompute(const ICompute &other) = delete;
		ICompute(ICompute &&other) = delete;
		~ICompute() override = default;
	};
} // namespace fragcore
#endif
