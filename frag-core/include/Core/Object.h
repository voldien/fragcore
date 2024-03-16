/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAG_CORE_OBJECT_H_
#define _FRAG_CORE_OBJECT_H_ 1
#include "../FragDef.h"
#include "UIDObject.h"
#include <string>

namespace fragcore {
	/**
	 * @brief Base abstract object for scene
	 * objects.
	 */
	class FVDECLSPEC Object : public UIDObject {
	  public:
		Object() = default;
		Object(const std::string &_name) { this->name = _name; }
		Object(Object &&other) { this->name = std::exchange(other.name, nullptr); }
		Object &operator=(Object &&other) {
			this->name = std::exchange(other.name, nullptr);
			return *this;
		}
		virtual ~Object() = default;

		virtual void setName(const std::string &name) { this->name.assign(name); }

		virtual void setName(std::string &&name) { this->name = std::move(name); }
		virtual void setName(const char *c_name) { this->name = c_name; }

		virtual std::string getName() noexcept { return this->name; }

		virtual const std::string &getName() const noexcept { return this->name; }

		template <typename T> inline T &as() noexcept { return static_cast<T &>(*this); }
		template <typename T> inline const T &as() const noexcept { return static_cast<const T &>(*this); }

	  protected: /*	*/
		Object &operator=(const Object &object) {
			this->setName(object.getName());
			return *this;
		}

		unsigned int type;

	  private: /*	Attributes.	*/
		std::string name;
	};
} // namespace fragcore

#endif
