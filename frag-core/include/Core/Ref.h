/**
	FragEngine, A Two layer Game Engine.
	Copyright (C) 2018  Valdemar Lindberg

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_REF_H_
#define _FRAG_CORE_REF_H_ 1
#include "../Def.h"
#include "SmartReference.h"
#include <utility>

namespace fragcore {
	/*  TODO fix!   */
	template <class T> class Ref { // TODO perhaps use the RefPtr as base?
	  private:
		T *reference;

	  public:
		FV_ALWAYS_INLINE bool operator==(const T *p_ptr) const { return reference == p_ptr; }

		FV_ALWAYS_INLINE bool operator!=(const T *p_ptr) const { return reference != p_ptr; }

		FV_ALWAYS_INLINE bool operator<(const Ref<T> &p_r) const { return reference < p_r.reference; }

		FV_ALWAYS_INLINE bool operator==(const Ref<T> &p_r) const { return reference == p_r.reference; }

		FV_ALWAYS_INLINE bool operator!=(const Ref<T> &p_r) const { return reference != p_r.reference; }

		FV_ALWAYS_INLINE T *operator->() { return reference; }

		FV_ALWAYS_INLINE T *operator*() { return reference; }

		FV_ALWAYS_INLINE const T *operator->() const { return reference; }

		FV_ALWAYS_INLINE const T *ptr() const { return reference; }

		FV_ALWAYS_INLINE T *ptr() { return reference; }

		FV_ALWAYS_INLINE const T *operator*() const { return reference; }

	  public:
		Ref() { reference = nullptr; }

		Ref(T *p_reference) {

			reference = nullptr;
			if (p_reference) {
				this->reference = p_reference;
				ref_pointer(p_reference);
			}
		}

		Ref(T &p_reference) {
			T *reference = nullptr;
			if (&p_reference != nullptr) {
				this->reference = &p_reference;
				ref_pointer(&p_reference);
			}
		}

		Ref(T &&other) { this->p_reference = std::exchange(other.p_reference, nullptr); }

		~Ref() { unref(); }

	  private:
		void ref_pointer(T *p_ref) {
			assert(p_ref);

			// if (p_ref->init_ref())
			//	reference = p_ref;
		}

		void unref() {
			if (this->reference && this->reference->deincreemnt()) {
				// delete reference;
				// memdelete(reference);
			}

			/*	*/
			this->reference = nullptr;
		}
	};
} // namespace fragcore
#endif
