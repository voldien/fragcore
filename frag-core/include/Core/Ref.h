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
#if !defined(_FRAGCORE_REF_H_) && !defined(FV_USE_SHARED_POINTER)
#define _FRAGCORE_REF_H_ 1
#include <cassert>
#include <utility>

namespace fragcore {
	/**
	 * @brief
	 *
	 * @tparam T
	 */
	template <class T> class Ref {
		static_assert(std::is_object_v<T>, "Must be a Object");

	  private:
		T *reference;

	  public:
		bool operator==(const T *p_ptr) const { return this->reference == p_ptr; }

		bool operator!=(const T *p_ptr) const { return this->reference != p_ptr; }

		bool operator<(const Ref<T> &p_r) const { return this->reference < p_r.reference; }

		bool operator==(const Ref<T> &p_r) const { return this->reference == p_r.reference; }

		bool operator!=(const Ref<T> &p_r) const { return this->reference != p_r.reference; }

		T *operator->() { return this->reference; }

		T *operator*() { return this->reference; }

		const T *operator->() const { return this->reference; }

		const T *ptr() const { return this->reference; }

		T *ptr() { return this->reference; }

		const T *operator*() const { return this->reference; }

		Ref() : reference(nullptr) {}

		Ref(T *p_reference) : reference(nullptr) {
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

		Ref(T &&other) {
			/*	*/
			this->reference = std::exchange(other.reference, nullptr);
		}

		Ref(const Ref &other) : Ref() {
			if (other.reference) {
				this->ref_pointer(other.reference);
			}
		}

		Ref &operator=(const Ref &other) {
			/*	*/
			if (other.reference) {
				this->ref_pointer(other.reference);
			}
			return *this;
		}

		Ref &operator=(Ref &&other) {
			this->reference = std::exchange(other.reference, nullptr);
			return *this;
		}

		~Ref() { this->unref(); }

	  private:
		void ref_pointer(T *p_ref) {
			assert(p_ref);

			this->reference = p_ref;
			this->reference->increment();
		}

		void unref() {
			if (this->reference) {
				/*	*/
				if (this->reference->deincreemnt()) {
					delete reference;
				}
			}

			/*	*/
			this->reference = nullptr;
		}
	};
} // namespace fragcore
#else
#include "../FragDef.h"
#endif
