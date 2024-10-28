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
#ifndef _FRAGCORE_REF_H_
#define _FRAGCORE_REF_H_ 1
#include "../FragDef.h"
#include <utility>

namespace fragcore {
	/**
	 * @brief
	 *
	 * @tparam T
	 */
	template <class T> class FVDECLSPEC Ref { // TODO perhaps use the RefPtr as base?
		static_assert(std::is_object<T>::value, "Must be a Object");
		// static_assert(std::has_virtual_destructor<T>::value, "Must have a virtual deconstructor");

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
		Ref() : reference(nullptr) {}

		Ref(T *p_reference) {

			reference = nullptr;
			if (p_reference) {
				this->ref_pointer(p_reference);
			}
		}

		Ref(T &p_reference) {
			T *reference = nullptr;
			if (&p_reference != nullptr) {
				this->ref_pointer(&p_reference);
			}
		}

		Ref(T &&other) { this->reference = std::exchange(other.reference, nullptr); }

		Ref(const Ref &other) : Ref() {
			if (other.reference) {
				this->ref_pointer(other.reference);
			}
		}

		Ref &operator=(const Ref &other) {
			if (other.reference) {
				this->ref_pointer(other.reference);
			}
			return *this;
		}

		~Ref() { unref(); }

	  private:
		void ref_pointer(T *p_ref) {
			assert(p_ref);

			// if (p_ref->init_ref())
			this->reference = p_ref;
			this->reference->increment();
			//	reference = p_ref;
		}

		void unref() {
			if (this->reference) {
				if (this->reference->deincreemnt()) {
					delete reference;
					// memdelete(reference);
				}
			}

			/*	*/
			this->reference = nullptr;
		}
	};
} // namespace fragcore
#endif
