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
#ifndef _FRAG_CORE_AUDIOCLIP_H_
#define _FRAG_CORE_AUDIOCLIP_H_ 1
#include "AudioDesc.h"
#include "AudioObject.h"
#include <FragCore.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC AudioClip : public AudioObject {
		friend class AudioInterface;

	  public:
		virtual unsigned long int getSampleRate() const;
		virtual unsigned long int getFrequency() const;
		virtual AudioFormat getAudioFormat() const;
		virtual unsigned int getNumberChannels() const;
		virtual unsigned long getSize() const;
		virtual float length() const;

		virtual AudioDataMode clipType() const;

		virtual void getData(void *pData, unsigned int nsamples, unsigned int offset);
		virtual void setData(const void *pData, unsigned int nsamples, unsigned int offset);

		intptr_t getNativePtr() const override;
		virtual ~AudioClip();

		virtual ~AudioClip();

	  protected:
		AudioClip();
	};
} // namespace fragcore

#endif
