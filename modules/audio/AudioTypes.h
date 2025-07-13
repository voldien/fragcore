/*
 *	FragCore - Fragment Core
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
#ifndef _FRAGCORE_AUDIO_TYPES_H_
#define _FRAGCORE_AUDIO_TYPES_H_ 1

namespace fragcore {

	enum AudioFormat {
		eMono,
		eStero,
	};

	enum class AudioSpeakerMode {
		Mono,
		Stero,
		Quad,
		Surround,
		Mode5Point1,
		Mode7Point1,
	};

	enum class AudioDataMode { LoadedInMemory, Streaming, DecompressOnLoad };

	typedef struct audio_capabilities_t {
		/*	*/
	} AudioCapabilities;

} // namespace fragcore
#endif
