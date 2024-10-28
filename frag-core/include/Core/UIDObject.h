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
#ifndef _FRAGCORE_UID_OBJECT_H_
#define _FRAGCORE_UID_OBJECT_H_ 1
#include "UIDGenerator.h"

namespace fragcore {

	/**
	 * @brief Responsible for containing
	 * the UID of object.
	 */
	class FVDECLSPEC UIDObject {
	  public:
		UIDObject() : UIDObject(UIDObject::generator) {}
		UIDObject(UIDGenerator<uint32_t> &uidGenerator) { this->setUID(uidGenerator.getNextUID()); }
		UIDObject(UIDObject &&other) = default;
		UIDObject(const UIDObject &other) = default;
		UIDObject &operator=(const UIDObject &) = default;
		UIDObject &operator=(UIDObject &&) = default;

		/**
		 * @brief Set Unique ID for the object.
		 *
		 * @param uid
		 */
		virtual void setUID(uint32_t uid) noexcept { this->uid = uid; }

		/**
		 *	@brief Get the unique identifier.
		 *	@return unique ID.
		 */
		virtual uint32_t getUID() const noexcept { return this->uid; }

		bool operator==(const UIDObject &other) noexcept {
			if (this == &other) {
				return true;
			}
			return this->uid == other.uid; // TODO verify!
		}
		bool operator!=(const UIDObject &other) noexcept { return !(*this == other); }

		static UIDGenerator<uint32_t> &getGenerator() noexcept { return generator; }

	  private: /*	Attributes.	*/
		// TODO remove. remove the responsiblity to the generator pool.
		static UIDGenerator<uint32_t> generator;
		uint32_t uid; /*	Unique Identifier.	*/
	};
} // namespace fragcore

#endif
