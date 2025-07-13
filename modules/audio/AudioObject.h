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
#ifndef _FRAGCORE_AUDIOOBJECT_H_
#define _FRAGCORE_AUDIOOBJECT_H_ 1
#include "AudioPrerequisites.h"
#include <Core/SmartReference.h>

namespace fragcore {
	class AudioInterface; // TODO relocate.
	// TODO add based on a dynamic interface object class. Similar to the rendering and possibliy physic.
	/**
	 *
	 */
	class AudioObject : public SmartReference {
	  public:
		AudioObject() = default;

		FV_ALWAYS_INLINE AudioInterface *getAudioInterface() const { return this->audioInterface; }

		FV_ALWAYS_INLINE void *getObject() { return this->pdata; }

		FV_ALWAYS_INLINE void *getObject() const { return this->pdata; }

		virtual intptr_t getNativePtr() const = 0;

	  protected: /*  */
		void *pdata;
		AudioInterface *audioInterface; // TODO change to ref.
	};
} // namespace fragcore

#endif
