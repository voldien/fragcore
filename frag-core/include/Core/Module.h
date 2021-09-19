
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
#ifndef _FRAG_CORE_MODULE_H_
#define _FRAG_CORE_MODULE_H_ 1
#include "Library.h"
#include "SmartReference.h"

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC Module : public SmartReference {
	  public:
		Module(void) = default;
		virtual ~Module(void) = default;
		virtual void OnInitialization(void) = 0;
		virtual void OnDestruction(void) = 0;
		/**/
	  public:
		// TODO determine where it shal lbe located
		static void loadModule(const char *name){

		}

		static void loadModule(Library &library) { library.getfunc("FragCoreModuleEntry"); }
	};
} // namespace fragcore

#endif
