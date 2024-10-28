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
#ifndef _FRAGCORE_PHYSIC_CONTACT_H_
#define _FRAGCORE_PHYSIC_CONTACT_H_ 1
#include <Math3D/Math3D.h>

namespace fragcore {

	class FVDECLSPEC Contact {
	  public:
		Contact() = default;
		Contact(const Contact &contact) = default;
		/**
		 *	@return
		 */
		virtual unsigned int getCount() const = 0;
		/**
		 *	@return
		 */
		virtual Vector3 impulse(int index) const = 0;
		/**
		 *	@return
		 */
		virtual Vector3 point(int index) const = 0;
		/**
		 *	@return
		 */
		virtual Vector3 normal(int index) const = 0;
		/**
		 *	@return
		 */
		virtual float distance(int index) const = 0;

	  private:				/*	Attributes.	*/
		void *pcontact;		/*	*/
		unsigned int count; /*	*/
	};
} // namespace fragcore

#endif
