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
#ifndef _FRAGCORE_PHYSICOBJECT_H_
#define _FRAGCORE_PHYSICOBJECT_H_ 1
#include "PhysicPrerequisites.h"
#include <Core/SmartReference.h>

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC PhysicObject : public SmartReference {
	  public:
		inline PhysicInterface *getPhysicInterface() const { return this->iRenderer; }

		inline void *getObject() noexcept { return this->pdata; }

		inline void *getObject() const noexcept { return this->pdata; }

		virtual intptr_t getNativePtr() const = 0;

	  protected: /*  */
		void *pdata;
		PhysicInterface *iRenderer; // TODO change to ref.
	};
} // namespace fragcore

#endif
