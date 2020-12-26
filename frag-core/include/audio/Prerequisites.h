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
#ifndef _FRAG_CORE_AUDIO_PREREQUISITES_H_
#define _FRAG_CORE_AUDIO_PREREQUISITES_H_ 1

namespace fragcore {
	class AudioInterface;
	class AudioClip;
	class AudioSource;
	class AudioListener;
	class AudioPhysicalDevice;
	class AudioSettings;
	class AudioObject;
	class AudioReverb;
    class AudioCapture;

    //TODO determine if need to be relocated to the frag preq
    class AudioDecoder;
    class AudioEncoder;
    class VorbisAudioDecoder;  
}

#endif
