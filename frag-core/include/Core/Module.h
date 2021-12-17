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
#ifndef _FRAG_CORE_MODULE_H_
#define _FRAG_CORE_MODULE_H_ 1
#include "Library.h"
#include "SmartReference.h"

namespace fragcore {
	class Module;
	typedef Module *(*CreateModule)(Module *module);
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Module : public SmartReference {
	  public:
		Module();
		virtual ~Module() = default;
		virtual void OnInitialization() = 0;
		virtual void OnDestruction() = 0;
		/**/
	  public:
		// TODO determine where it shal lbe located
		static Module *loadModule(const std::string &moduleName, const std::string &moduleEntryPoint = "") {
			auto moduleFile = getModuleName(moduleName);
			Library lib(moduleFile.c_str());
			return Module::loadModule(lib, moduleEntryPoint);
		}

		static Module *loadModule(Library &library, const std::string &moduleEntryPoint = "") {
			Module *module;
			if (moduleEntryPoint.empty()) {
				//				moduleEntryPoint = getModuleName(moduleEntryPoint);
			}

			CreateModule createModuleFunc = nullptr;
			if (!library.isValid()) {
			}

			createModuleFunc = library.getfunc<CreateModule>("FragCoreModuleEntry");
			module = createModuleFunc(nullptr);

			return module;
		}
		static std::string getModuleName(const std::string &name);
	};
} // namespace fragcore

#endif
