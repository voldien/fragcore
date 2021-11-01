/**
	FragEngine, A Two layer Game Engine.
	Copyright (C) 2014  Valdemar Lindberg

	This getProgram() is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This getProgram() is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this getProgram().  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_VIDEO_TEXTURE_H_
#define _FRAG_CORE_VIDEO_TEXTURE_H_ 1
#include "VideoDecoder.h"
#include <AudioClip.h>
#include <Core/SmartReference.h>
#include <Core/TaskScheduler/IScheduler.h>
#include <Texture.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC VideoTexture : public SmartReference {
		friend class VideoFactory;
		friend class VideoManager;

	  public:
		VideoTexture();

		~VideoTexture();

		virtual bool isPlaying() const;

		virtual void play();

		virtual void stop();

		virtual void pause();

		virtual float getCurrentPosition() const;

		/*	Total length in seconds*/
		virtual float getTotalLength() const;

		virtual double getDuration() const;

		virtual double getTime() const;

		virtual bool isReadyToPlay() const;

		virtual Texture *getTexture() const;

	  protected:
		Task::TaskCallBack taskcallback;
		void *pVideoData;
		Ref<AudioClip> audioClip;
		Ref<Texture> texture;
		Ref<VideoDecoder> decoder;
	};
} // namespace fragcore

#endif
