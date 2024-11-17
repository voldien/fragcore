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
#ifndef _FRAGCORE_AUDIODESC_H_
#define _FRAGCORE_AUDIODESC_H_ 1
#include "AudioTypes.h"
#include <FragDef.h>
#include <IO/IO.h>
#include <Math/Math.h>

namespace fragcore {

	using AudioSourceDesc = struct audio_source_desc_t {
		Vector3 position;
	};

	class AudioDecoder;
	using AudioClipDesc = struct audio_clip_desc_t {
		/*  */
		void *source;		  /*  */
		size_t size;		  /*  */
		size_t sampleRate;	  /*  */
		AudioFormat format;	  /*  */
		unsigned int samples; /*  */

		/*  */
		bool streaming;			   // TODO change to a enum.
		Ref<AudioDecoder> decoder; /*	*/
		AudioDataMode datamode;	   /*	*/
	};

	using AudioListenerDesc = struct audio_listener_desc_t {
		Vector3 position;
		Quaternion rotation;

	};

	using AudioReverbDesc = struct audio_reverb_desc_t {

	};

	using AudioGeometryDesc = struct audio_geometry_desc_t {

	};

	using AudioCaptureDesc = struct audio_capture_desc_t {
		char *device;
		AudioFormat format;	  /*  */
		unsigned int samples; /*  */
	};

} // namespace fragcore

#endif
