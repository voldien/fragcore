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
#ifndef _FRAG_CORE_AUDIOREVERB_H_
#define _FRAG_CORE_AUDIOREVERB_H_ 1

#include "AudioObject.h"

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC AudioReverb : public AudioObject {
		friend class AudioInterface;

	  public:
		intptr_t getNativePtr() const override;

	  protected:
		AudioReverb();
		virtual ~AudioReverb();
	};

} // namespace fragcore

#endif
