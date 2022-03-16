#include "../AudioDesc.h"
#include "internal_object_type.h"

namespace fragcore {

	const char *openAlErrorToString(int err) {
		switch (err) {
		case AL_NO_ERROR:
			return "AL_NO_ERROR";
		case AL_INVALID_ENUM:
			return "AL_INVALID_ENUM";
		case AL_INVALID_VALUE:
			return "AL_INVALID_VALUE";
		case AL_INVALID_OPERATION:
			return "AL_INVALID_OPERATION";
		case AL_OUT_OF_MEMORY:
			return "AL_OUT_OF_MEMORY";
		default:
			return "Unknown error code";
		}
	}
	// TODO perhaps move to a helper header file and etc.

	ALenum to_al_format(short channels, short samples) {
		bool stereo = (channels > 1);
		//	AL_FORMAT_STER
		switch (samples) {
		case 32:
			if (stereo) {
				return AL_FORMAT_STEREO_FLOAT32;
			} else {
				return AL_FORMAT_MONO_FLOAT32;
			}
		case 16:
			if (stereo) {
				return AL_FORMAT_STEREO16;
			} else {
				return AL_FORMAT_MONO16;
			}
		case 8:
			if (stereo) {
				return AL_FORMAT_STEREO8;
			} else {
				return AL_FORMAT_MONO8;
			}
		default:
			throw RuntimeException("Invalid format. ");
		}
	}

	ALenum translate2ALFormat(AudioFormat format) {
		switch (format) {
		case eStero:
			return 0;
		default:
			return 0;
		}
	}
} // namespace fragcore