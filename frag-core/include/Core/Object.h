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
#ifndef _FRAGCORE_OBJECT_H_
#define _FRAGCORE_OBJECT_H_ 1
#include "../FragDef.h"
#include "UIDObject.h"

namespace fragcore {

	/**
	 * @brief Base abstract object for scene
	 * objects.
	 */
	class FVDECLSPEC Object : public UIDObject {
		// using
		// TODO: declare UID generator namespace.
	  public:
		Object() = default;
		Object(const std::string &_name) : name(_name) {}
		Object(Object &&other) { this->name = std::exchange(other.name, nullptr); }
		Object(const Object &other) = default;
		Object &operator=(Object &&other) {
			UIDObject::operator=(other);
			this->name = std::exchange(other.name, nullptr);
			return *this;
		}
		virtual ~Object() = default;

		virtual void setName(const std::string &name) { this->name.assign(name); }
		virtual void setName(std::string &&name) { this->name = std::move(name); }
		virtual void setName(const char *c_name) { this->name = c_name; }

		virtual std::string getName() noexcept { return this->name; }
		virtual const std::string &getName() const noexcept { return this->name; }

		template <typename T> T &as() noexcept { return static_cast<T &>(*this); }
		template <typename T> const T &as() const noexcept { return static_cast<const T &>(*this); }

	  protected: /*	*/
		Object &operator=(const Object &object) {
			this->setName(object.getName());
			return *this;
		}

	  private: /*	Attributes.	*/
		std::string name;
	};
} // namespace fragcore

#endif
