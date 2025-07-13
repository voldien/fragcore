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
#ifndef _FRAGCORE_OPENAL_AUDIOLISTENER_H_
#define _FRAGCORE_OPENAL_AUDIOLISTENER_H_ 1
#include "../AudioListener.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC OpenALAudioListener : public AudioListener {
		friend class AudioInterface;

	  public:
		virtual ~OpenALAudioListener();
		OpenALAudioListener();

		/**
		 * @brief Set the Volume object
		 *
		 * @param volume
		 */
		virtual void setVolume(float volume) override;

		/**
		 * @brief Get the Volume object
		 *
		 * @return float
		 */
		virtual float getVolume() const override;

		// TODO rename to mute
		/**
		 * @brief
		 *
		 * @param pause
		 */
		virtual void pause(bool pause) override;

		// virtual void getOutPutData();

		virtual void setPosition(const Vector3 &position) override;
		virtual const Vector3 getPosition() const override;

		virtual void setVelocity(const Vector3 &velocity) override;
		virtual const Vector3 getVelocity() const override;

		virtual void setOrientation(const Quaternion &orientation) override;
		virtual const Quaternion getOrientation() const override;

		virtual intptr_t getNativePtr() const override;
	};
} // namespace fragcore

#endif
