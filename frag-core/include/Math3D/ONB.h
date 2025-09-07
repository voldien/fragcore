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
#ifndef _FRAGCORE_ONB_H_
#define _FRAGCORE_ONB_H_ 1
#include "../Math/Math.h"

namespace fragcore {

	/**
	 *	ORTHONORMAL BASED AND FRAMES
	 */
	struct FVDECLSPEC ONB {
	  public:
		ONB() = default;
		ONB(const ONB &other) { *this = other; }
		ONB(const Vector3 &a, const Vector3 &b, const Vector3 &c) { this->set(a, b, c); }

		/**
		 *
		 */
		void initFromU(const Vector3 &u) noexcept {
			Vector3 n(1.0f, 0.0f, 0.0f);
			Vector3 m(0.0f, 1.0f, 0.0f);

			this->setU(u.normalized());
			this->setV(this->u().cross(n));
			if (this->v().norm() < Math::Epsilon) {
				this->setV(this->u().cross(m));
			}
			this->setW(this->u().cross(this->v()));
		}
		void initFromV(const Vector3 &v) noexcept {
			Vector3 n(1.0f, 0.0f, 0.0f);
			Vector3 m(0.0f, 1.0f, 0.0f);

			this->setV(v.normalized());
			this->setU(this->v().cross(n));
			if (this->u().norm() < Math::Epsilon) {
				this->setU(this->v().cross(m));
			}
			this->setW(this->u().cross(this->v()));
		}

		void initFromW(const Vector3 &w) noexcept {
			Vector3 n(1.0f, 0.0f, 0.0f);
			Vector3 m(0.0f, 1.0f, 0.0f);

			this->setW(w.normalized());
			this->setU(this->w().cross(n));
			if (this->u().norm() < Math::Epsilon) {
				this->setU(this->w().cross(m));
			}
			this->setW(this->w().cross(this->v()));
		}

		/**
		 *
		 */
		void set(const Vector3 &a, const Vector3 &b, const Vector3 &c) {
			this->m[0] = a;
			this->m[1] = b;
			this->m[2] = c;
		}
		void setU(const Vector3 &u) { this->m[0] = u; }
		void setV(const Vector3 &v) { this->m[1] = v; }
		void setW(const Vector3 &w) { this->m[2] = w; }

		/**
		 *
		 */
		void initFromUV(const Vector3 &u, const Vector3 &v) noexcept { this->m[2] = u.cross(v); }
		void initFromVU(const Vector3 &v, const Vector3 &u) noexcept { this->m[2] = v.cross(u); }

		/**
		 *
		 */
		void initFromUW(const Vector3 &u, const Vector3 &w) noexcept { this->m[1] = u.cross(w); }
		void initFromWU(const Vector3 &w, const Vector3 &u) noexcept { this->m[1] = w.cross(u); }

		/**
		 *
		 */
		void initFromVW(const Vector3 &v, const Vector3 &w) noexcept { this->m[0] = v.cross(w); }
		void initFromWV(const Vector3 &w, const Vector3 &v) noexcept { this->m[0] = w.cross(v); }

		/**
		 *
		 * @param is
		 * @param t
		 * @return stream reference.
		 */
		// friend std::istream &operator>>(std::istream &is, ONB &t){

		// }

		/**
		 *
		 * @param os
		 * @param t
		 * @return
		 */
		friend std::ostream &operator<<(std::ostream &os, const ONB &t) {
			os << t.u() << t.v() << t.w();
			return os;
		}

		/**
		 *	Compare
		 * @return true if equal.
		 */
		friend bool operator==(const ONB &o1, const ONB &o2) {
			return (o1.u() == o2.u()) && (o1.v() == o2.v()) && (o1.w() == o2.w());
		}

		/**
		 *	Compare
		 * @return true if equal.
		 */
		friend bool operator!=(const ONB &o1, const ONB &o2) {
			return (o1.u() != o2.u()) && (o1.v() != o2.v()) && (o1.w() != o2.w());
		}

		ONB &operator=(const ONB &onb) {
			setU(onb.u());
			setV(onb.v());
			setW(onb.w());
			return *this;
		}

		/**
		 * @return
		 */
		 const Vector3 &u() const noexcept { return this->m[0]; }

		/**
		 * @return
		 */
		 const Vector3 &v() const noexcept { return this->m[1]; }

		/**
		 * @return
		 */
		 const Vector3 &w() const noexcept { return this->m[2]; }

	  private:		  /*	Attributes.	*/
		Vector3 m[3]; /*	*/
	};
} // namespace fragcore

#endif
