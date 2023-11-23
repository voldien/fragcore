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

#ifndef _FRAG_CORE_IFILE_NOTIFY_H_
#define _FRAG_CORE_IFILE_NOTIFY_H_ 1
#include "../../FragDef.h"
#include "Core/Object.h"
#include "FileChangeEvent.h"

namespace fragcore {

	// TOOD Determine name if shall only be INotify
	class FVDECLSPEC IFileNotify : public SmartReference {
	  public:
		/**
		 *
		 */
		class FVDECLSPEC FileNoticationEntry {
		  public:
			int key;			  /*  */
			std::string filepath; /*  */
			Object *assetObject;  /*  */
			void *userdata;
			// TOOD change!
			// AssetType type;         /*  */
		};

		virtual ~IFileNotify() {}

		/**
		 * @brief
		 *
		 * @param path
		 * @param object
		 */
		virtual void addFilePath(const char *path, Object *object) = 0;

		/**
		 * @brief
		 *
		 * @param path
		 * @param object
		 */
		virtual void removeFilePath(const char *path, Object *object) = 0;

		/**
		 * @brief Get the Object object
		 *
		 * @param path
		 * @return Object*
		 */
		virtual Object *getObject(const char *path) = 0;

		/**
		 * @brief Get the Entry object
		 *
		 * @param object
		 * @return FileNoticationEntry*
		 */
		FileNoticationEntry *getEntry(Object *object);
	};
} // namespace fragcore
#endif
