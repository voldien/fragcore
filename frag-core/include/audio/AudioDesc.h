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
#ifndef _FRAG_CORE_AUDIODESC_H_
#define _FRAG_CORE_AUDIODESC_H_ 1
#include "../Def.h"
#include"AudioTypes.h"
#include "../Core/IO/IO.h"
#include "../Core/Ref.h"

namespace fragcore
{
	typedef struct audio_source_desc_t
	{
		PVVector3 position;
	} AudioSourceDesc;

	class AudioDecoder;
	typedef struct audio_clip_desc_t
	{
		/*  */
		void *source;				/*  */
		size_t size;				/*  */
		int sampleRate; 			/*  */
		AudioFormat format;			/*  */
		unsigned int samples;		/*  */

		/*  */
		bool streaming;		  		//TODO change to a enum.
		Ref<AudioDecoder> decoder;	/*	*/
		AudioDataMode datamode;		/*	*/
	} AudioClipDesc;

	typedef struct audio_listener_desc_t
	{
		PVVector3 position;
		PVQuaternion rotation;

	} AudioListenerDesc;

	typedef struct audio_reverb_desc_t
	{

	} AudioReverbDesc;

	typedef struct audio_geometry_desc_t
	{

	} AudioGeometryDesc;

}

#endif