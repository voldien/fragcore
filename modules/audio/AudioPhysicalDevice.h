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
#ifndef _FRAGCORE_AUDIO_PHYSICAL_DEVICE_H_
#define _FRAGCORE_AUDIO_PHYSICAL_DEVICE_H_ 1
#include <Core/Object.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC AudioPhysicalDevice : public Object {
	  public:
		AudioPhysicalDevice() = default;
		AudioPhysicalDevice(const AudioPhysicalDevice &other) : Object(other) { this->setName(other.getName()); }
		AudioPhysicalDevice(AudioPhysicalDevice &&other) : Object(other) {}
		~AudioPhysicalDevice() override = default;

		// TODO perhaps use the audio setting as part of the audio device.
		// virtual AudioSettings* getAudioSettings() = 0;
		// virtual void getCapabilities() = 0;
	  private:
	};
} // namespace fragcore

#endif
