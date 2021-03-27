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
#include"../Def.h"
#include"UIDObject.h"
#include<string>
#include<string.h>

namespace fragcore {
	/**
	 *	Base abstract object for scene
	 *	objects.
	 */
	class FVDECLSPEC Object : public UIDObject {
	public:

		inline Object(void) noexcept {

		}

		virtual void setName(const std::string &name) {
			this->name.assign(name);
		}
		virtual void setName(const std::string&& name){
			this->name = name;
		}

		virtual std::string getName(void) {
			return this->name;
		}

		virtual const std::string& getName(void) const{
			return this->name;
		}

	protected:	/*	*/

		Object& operator=(const Object& object){
			this->setName(object.getName());
			this->name = object.name;
			return *this;
		}

		unsigned int type;
	private:	/*	Attributes.	*/

		std::string name;
	};
}

#endif
