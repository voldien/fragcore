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
#ifndef _FRAGCORE_MODULE_H_
#define _FRAGCORE_MODULE_H_ 1
#include "Library.h"
#include "SmartReference.h"

namespace fragcore {
	class Module;
	using CreateModule = Module *(*)(Module *);

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Module : public SmartReference {
	  public:
		Module();
		~Module() override = default;
		virtual void onInitialization() = 0;
		virtual void onDestruction() = 0;

	  public: /*	*/
		template <typename T> static T *loadModule(const std::string &filepath, const std::string &moduleEntryPoint = "") {
			static_assert(std::is_convertible_v<T *, Module *>, "Derived must inherit Module as public");
			Library lib(filepath.c_str());
			return Module::loadModule<T>(lib, moduleEntryPoint);
		}

		template <typename T> static T *loadModule(Library &library, const std::string &moduleEntryPoint) {
			static_assert(std::is_convertible_v<T *, Module *>, "Derived must inherit Module as public");
			T *module = nullptr;
			/*	*/
			std::string moduleEntry = moduleEntryPoint;

			if (moduleEntry.empty()) {
				moduleEntry = Module::getDefaultModuleName(library.getPath());
			}

			CreateModule createModuleFunc = nullptr;
			if (!library.isValid()) {
				throw RuntimeException("Invalid library {}", library.getPath());
			}

			createModuleFunc = library.getfunc<CreateModule>(moduleEntry.c_str());
			if (createModuleFunc == nullptr) {
				throw RuntimeException("Module Entrypoint {} not found for module", moduleEntry, library.getPath());
			}
			module = dynamic_cast<T *>(createModuleFunc(nullptr));

			module->onInitialization();

			return module;
		}

		static std::string getDefaultModuleName(const std::string &name);
	};
} // namespace fragcore

#endif
