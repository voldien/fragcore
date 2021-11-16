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
#ifndef _FRAG_CORE_BASE32_H_
#define _FRAG_CORE_BASE32_H_ 1
#include "IBase.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Base32 : public IBase {
	  public:
		virtual void encode(Ref<IO> &input, Ref<IO> &encoded) override;
		virtual void decode(Ref<IO> &encoded, Ref<IO> &input) override;
	};
} // namespace fragcore

#endif
