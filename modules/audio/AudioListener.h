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
#ifndef _FRAGCORE_AUDIOLISTENER_H_
#define _FRAGCORE_AUDIOLISTENER_H_ 1
#include "AudioObject.h"
#include <FragCore.h>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC AudioListener : public AudioObject {
		friend class AudioInterface;

	  public:
		virtual ~AudioListener() = default;
		AudioListener() = default;

		/**
		 * @brief Set the Volume object
		 *
		 * @param volume
		 */
		virtual void setVolume(float volume) = 0;

		/**
		 * @brief Get the Volume object
		 *
		 * @return float
		 */
		virtual float getVolume() const = 0;

		// TODO rename to mute
		/**
		 * @brief
		 *
		 * @param pause
		 */
		virtual void pause(bool pause) = 0;

		// virtual void getOutPutData();

		virtual void setPosition(const Vector3 &position) = 0;
		virtual const Vector3 getPosition() const = 0;

		virtual void setVelocity(const Vector3 &velocity) = 0;
		virtual const Vector3 getVelocity() const = 0;

		virtual void setOrientation(const Quaternion &orientation) = 0;
		virtual const Quaternion getOrientation() const = 0;
	};
} // namespace fragcore

#endif
