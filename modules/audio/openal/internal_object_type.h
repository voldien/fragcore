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
#ifndef _FRAG_CORE_INTERNAL_OBJECT_TYPE_H_
#define _FRAG_CORE_INTERNAL_OBJECT_TYPE_H_ 1
#include "../AudioObject.h"
#include "../AudioTypes.h"
#include "../decoder/AudioDecoder.h"
#include <AL/al.h>
#include <AL/alext.h>
#include <AL/alut.h>
#include <AL/efx-presets.h>

namespace fragcore {

	// typedef struct openal_audio_capture_t {
	// 	friend class OpenALAudioInterface;
	// 	unsigned int source;
	// } ALAudioCapture;

	extern FVDECLSPEC ALenum translate2ALFormat(AudioFormat format);
	extern FVDECLSPEC ALenum to_al_format(short channels, short samples);
	extern FVDECLSPEC const char *openAlErrorToString(int err);
#define FAOPAL_VALIDATE(x)                                                                                             \
	do {                                                                                                               \
		x;                                                                                                             \
		ALenum _err = alGetError();                                                                                    \
		if (_err != AL_NO_ERROR) {                                                                                     \
			throw cxxexcept::RuntimeException("{} {} {} - {}", __FILE__, __LINE__, _err, openAlErrorToString(_err));   \
		}                                                                                                              \
	} while (0)

} // namespace fragcore

#endif
