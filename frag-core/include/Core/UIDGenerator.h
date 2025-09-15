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
#ifndef _FRAGCORE_UID_GENERATOR_H_
#define _FRAGCORE_UID_GENERATOR_H_ 1
#include "../FragDef.h"
#include <atomic>

namespace fragcore {

	/**
	 *	@brief Unique Identifier generator.
	 *	This is a simple UID generator.
	 *	Each next will be an increment of previosuly.
	 */
	template <typename T = uint32_t> class FVDECLSPEC UIDGenerator {
	  public:
		using UIDType = T;

		static_assert(std::is_arithmetic_v<UIDType>, "Must support artithmetic operations");
		UIDGenerator() noexcept { this->nextUID = 0; }
		UIDGenerator(UIDGenerator &&other) {
			this->nextUID.store(other.nextUID);
			this->uid.store(other.uid);
		}
		UIDGenerator(const UIDGenerator &other) {
			this->nextUID.store(other.nextUID);
			this->uid.store(other.uid);
		}
		~UIDGenerator() = default;

		UIDGenerator &operator=(const UIDGenerator &other) {
			this->nextUID.store(other.nextUID);
			this->uid.store(other.uid);
			return *this;
		}
		UIDGenerator &operator=(UIDGenerator &&other) {
			this->nextUID.store(other.nextUID);
			this->uid.store(other.uid);
			return *this;
		}

		/**
		 * @brief Get the Next U I D object
		 *
		 * @return next unique id.
		 */
		UIDType getNextUID() noexcept { return this->nextUID.fetch_add(1, std::memory_order_relaxed); }

		bool operator==(const UIDGenerator &other) noexcept {
			if (this == &other) {
				return true;
			}
			return this->uid == other.uid;
		}
		bool operator!=(const UIDGenerator &other) noexcept { return !(*this == other); }

	  private:						  /*	Attributes.	*/
		std::atomic<UIDType> nextUID; /*	*/
		std::atomic<UIDType> uid;	  // TODO resolve.
	};

} // namespace fragcore

#endif
