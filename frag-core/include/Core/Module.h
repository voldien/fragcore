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

	  public: /*	*/
		static Module *loadModule(const std::string &path, const std::string &moduleEntryPoint = "") {
			Library lib(path.c_str());
			return Module::loadModule(lib, moduleEntryPoint);
		}

		static Module *loadModule(Library &library, const std::string &moduleEntryPoint) {
			Module *module;
			/*	*/
			std::string moduleEntry = moduleEntryPoint;

			if (moduleEntry.empty()) {
				moduleEntry = getDefaultModuleName(library.getPath());
			}

			CreateModule createModuleFunc = nullptr;
			if (!library.isValid()) {
				throw RuntimeException("Invalid library {}", library.getPath());
			}

			createModuleFunc = library.getfunc<CreateModule>(moduleEntry.c_str());
			if (createModuleFunc == nullptr) {
				throw RuntimeException("Module Entrypoint {} not found for module", moduleEntry, library.getPath());
			}
			module = createModuleFunc(nullptr);

			module->OnInitialization();

			return module;
		}
		static std::string getDefaultModuleName(const std::string &name);
	};
} // namespace fragcore

#endif
