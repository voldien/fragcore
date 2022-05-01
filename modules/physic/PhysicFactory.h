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
#ifndef _FRAG_CORE_PHYSIC_FACTORY_H_
#define _FRAG_CORE_PHYSIC_FACTORY_H_ 1
#include "PhysicInterface.h"

namespace fragcore {
	/**
	 *	Responsible for creating the physic interface.
	 */
	class FVDECLSPEC PhysicFactory {
	  public:
		/*  Official physic api.  */
		enum class PhysicAPI {
			Bullet, /*	Bullet physic API.	*/
			PhysX,	/*	Nvidia PhysicX.	*/
			RPC,
		};

		/**
		 *	Create PhysicInterface object based on what API.
		 *
		 *	\api
		 *
		 *	\connection will connect to remote server. if NULL
		 *	it will attempt to create a local connection by creating
		 *	a physic interface that does not uses the socket communication.
		 *
		 *	\resource
		 *
		 */
		static PhysicInterface *createPhysic(PhysicAPI api, IConfig *overrideOption);

		/**
		 *	Load physicinterface by its library filename.
		 *
		 *	\libpath
		 *
		 *	\connection
		 *
		 */
		static PhysicInterface *createPhysic(const char *libpath, IConfig *overrideOption);

		/**
		 * @return
		 */
		static const char *getInterfaceLibraryPath(PhysicAPI api);

	  private: /*	Prevent one from creating an instance of this class.	*/
		PhysicFactory();
		PhysicFactory(const PhysicFactory &other);
		PhysicFactory(PhysicFactory &&other);
	};
} // namespace fragcore

#endif
