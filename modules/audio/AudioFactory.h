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
#ifndef _FRAGCORE_AUDIOFACTORY_H_
#define _FRAGCORE_AUDIOFACTORY_H_ 1
#include "AudioInterface.h"

namespace fragcore {
	class FVDECLSPEC AudioFactory {
	  public:
		enum AudioAPI {			/*  Official audio api.  */
						OpenAL, /*	*/
						FMOD,	/*  */
		};

		/**
		 * Create audio interface
		 * @param audioapi
		 * @param resources
		 * @param config
		 * @return none NULL if successful.
		 */
		static AudioInterface *createAudioInterface(AudioAPI audioapi, IConfig *config);

		/**
		 * Create rendering interface
		 * @param cpathlib
		 * @param resources
		 * @param config
		 * @return none NULL if successful.
		 */
		static AudioInterface *createAudioInterface(const char *cpathlib, IConfig *config);

		/**
		 * Get rendering interface library file path.
		 * @param api valid api.
		 * @return non-null terminated string.
		 */
		static const char *getInterfaceLibraryPath(AudioAPI api);

	  private: /*	Prevent one from creating an instance of this class.	*/
		AudioFactory();

		AudioFactory(const AudioFactory &other);
	};
} // namespace fragcore

#endif
