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
#ifndef _FRAG_CORE_FILEACCESS_H_
#define _FRAG_CORE_FILEACCESS_H_ 1
#include "../SmartReference.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC FileAccess : public SmartReference {
	  public:
		FileAccess();
		~FileAccess();

		virtual bool readable() const;
		virtual bool writeable() const;
		virtual bool executable() const;

		//		virtual uint32_t _get_unix_permissions() = 0;
		//		virtual void _set_unix_permissions(uint32_t p_permissions) = 0;
	};
} // namespace fragcore

#endif
