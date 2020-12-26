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
#ifndef _FRAG_CORE_IBASE_H_
#define _FRAG_CORE_IBASE_H_ 1
#include"IO/IO.h"
#include"Ref.h"

namespace fragcore
{
	class FVDECLSPEC IBase {
	public:
		virtual ~IBase(void) = 0;
		virtual void encode(Ref<IO>& raw, Ref<IO>& encoded) = 0;
		virtual void decode(Ref<IO>& encoded, Ref<IO>& raw) = 0;
	};
}
#endif