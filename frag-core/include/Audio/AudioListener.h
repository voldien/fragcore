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
#ifndef _FRAG_CORE_AUDIOLISTENER_H_
#define _FRAG_CORE_AUDIOLISTENER_H_ 1
#include"../Def.h"
#include "AudioObject.h"

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC AudioListener : public AudioObject {
		friend class AudioInterface;
	public:

		virtual void setVolume(float volume);
		virtual float getVolume() const;

		//TODO rename to mute
		virtual void pause(bool pause);

		//virtual void getOutPutData();

		virtual void setPosition(const Vector3& position);
		virtual const Vector3 getPosition() const;

		virtual void setVelocity(const Vector3& velocity);
		virtual const Vector3 getVelocity() const;

		virtual void setOrientation(const Quaternion& orientation);
		virtual const Quaternion getOrientation() const;

		virtual intptr_t getNativePtr() const override;

	protected:
		AudioListener();
		virtual ~AudioListener();
	};
}

#endif
