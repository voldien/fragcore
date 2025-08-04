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
#ifndef _FRAGCORE_ORIENTED_BOUNDING_BOX_H_
#define _FRAGCORE_ORIENTED_BOUNDING_BOX_H_ 1
#include "ONB.h"

namespace fragcore {
	/**
	 *	Oriented bounding box.
	 */
	struct FVDECLSPEC OBB {
	  public:
		OBB() = default;

		OBB(const OBB &obb) { *this = obb; }

		OBB(const Vector3 &u, const Vector3 &v, const Vector3 &w) { this->onb.set(u, v, w); }

		OBB(const Vector3 &u, const Vector3 &v, const Vector3 &w, const Vector3 &size) {
			this->onb.set(u, v, w);
			this->setSize(size);
		}

		// OBB(const Vector3 &u, const Vector3 &v, const Vector3 &w, const Vector3 &center, const Vector3 &size) {}

		/**
		 * Get U axis.
		 * @return normalized axis.
		 */
		const Vector3 &getU() const { return onb.u(); }

		/**
		 *	Get V axis.
		 * @return normalized axis.
		 */

		/**
		 *
		 * @return
		 */
		const Vector3 &getV() const { return onb.v(); }

		/**
		 * Get W axis.
		 * @return normalized axis.
		 */
		const Vector3 &getW() const { return onb.w(); }

		/**
		 * Set size of box.
		 * @param size
		 */
		void setSize(const Vector3 &size) { this->size = size; }

		/**
		 * Get half size.
		 * @return non-negative vector of the size.
		 */
		 Vector3 getSize() { return size; }

		/**
		 *
		 * @return
		 */
		 const Vector3 &getSize() const { return this->size; }

		OBB &operator=(const OBB &obb) {
			this->onb = obb.onb;
			this->setSize(obb.getSize());
			return *this;
		}

	  private:		  /*	Attributes.	*/
		ONB onb;	  /*	*/
		Vector3 size; /*	*/
	};
} // namespace fragcore

#endif
