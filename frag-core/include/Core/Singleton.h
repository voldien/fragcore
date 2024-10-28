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
#ifndef _FRAGCORE_SINGLETON_H_
#define _FRAGCORE_SINGLETON_H_ 1
#include "../FragDef.h"
#include "Ref.h"
#include "SmartReference.h"

namespace fragcore {
	
	/**
	 * @brief
	 *
	 * @tparam T
	 */
	template <typename T> class FVDECLSPEC Singleton : public SmartReference {
	  public:
	  
	  	/**
	  	 * @brief Get the Instance object
	  	 * 
	  	 * @return Ref<T> 
	  	 */
		static Ref<T> getInstance() {
			static T *_instance = nullptr;
			if (_instance == nullptr) {
				T *instance = new T();
				Ref<T> ref = Ref<T>(instance);
				instance->onCreation(ref);
			}
			return Ref<T>(_instance);
		}

		/**
		 * @brief 
		 * 
		 * @param instance 
		 */
		virtual void onCreation(Ref<T> &instance) = 0;
	};
} // namespace fragcore
#endif
