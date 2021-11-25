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
#ifndef _FRAG_CORE_OBJECT_H_
#define _FRAG_CORE_OBJECT_H_ 1
#include "../Def.h"
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
		Object(const std::string &name) { setName(name); }
		Object(Object &&other) { this->name = std::exchange(other.name, nullptr); }
		Object &operator=(Object &&other) {
			this->setName(std::move(other.name));
			return *this;
		}
		~Object() = default;

		virtual void setName(const std::string &name) { this->name.assign(name); }

		virtual void setName(std::string &&name) { this->name = std::move(name); }
		virtual void setName(const char *name) { this->name.assign(name); }

		virtual std::string getName() noexcept { return this->name; }

		virtual const std::string &getName() const noexcept { return this->name; }

	  protected: /*	*/
		Object &operator=(const Object &object) {
			this->setName(object.getName());
			this->name = object.name;
			return *this;
		}

		unsigned int type;

	  private: /*	Attributes.	*/
		std::string name;
	};
} // namespace fragcore

#endif
