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
#ifndef _FRAG_CORE_SINGLETON_H_
#define _FRAG_CORE_SINGLETON_H_ 1
#include "../Def.h"
#include "SmartReference.h"

namespace fragcore {
	template <typename T> class FVDECLSPEC Singleton : public SmartReference {
	  public:
		static Ref<T> getInstance(void) {
			static T *_instance = nullptr;
			if (_instance == nullptr) {
				T *instance = new T();
				Ref<T> ref = Ref<T>(instance);
				instance->onCreation(ref);
			}
			return Ref<T>(_instance);
		}

		virtual void onCreation(Ref<T> &instance) = 0;
	};
} // namespace fragcore
#endif
