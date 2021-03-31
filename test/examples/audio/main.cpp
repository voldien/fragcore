#include <Audio/AudioClip.h>
#include <Audio/AudioListener.h>
#include <Audio/AudioSource.h>
#include <Audio/decoder/AudioDecoder.h>
#include <Audio/decoder/VorbisDecoder.h>
#include <FragCore.h>
// #include <ogg/ogg.h>
// #include <opus/opus.h>

using namespace fragcore;

class AudioPlayback {
  public:
	AudioPlayback(int argc, const char **argv) {
		IConfig config;

		if (argc < 2)
			throw InvalidArgumentException("Required path to a audio file");

		Ref<AudioInterface> iaudio =
			Ref<AudioInterface>(AudioFactory::createAudioInterface(AudioFactory::OpenAL, &config));

		/*  Initialize HPM.  */
		if (!LIBHPM::Hpm::init(LIBHPM::Hpm::eHPM_SSE2))
			throw RuntimeException("Failed to initialize the hpm library.");

		Ref<IScheduler> sch = Ref<IScheduler>(NULL);
		FileSystem *fileSystem = FileSystem::createFileSystem(sch);

		std::vector<AudioPhysicalDevice> devices = iaudio->getDevices();
		std::vector<AudioPhysicalDevice>::iterator it = devices.begin();

		for (; it != devices.end(); it++) {
			printf(fvformatf("%s\n", (*it).getName().c_str()).c_str());
		}
		// printf("Current Device: %s", iaudio->getAudioDevice().getName());

		AudioListenerDesc list_desc = {.position = Vector3(0, 0, 0), .rotation = Quaternion::identity()};
		list_desc.position[10] = 0;
		Ref<AudioListener> listener = Ref<AudioListener>(iaudio->createAudioListener(&list_desc));
		listener->setVolume(1.0f);
		AudioSourceDesc source_desc = {};
		source_desc.position = Vector3::zero();
		Ref<AudioSource> audioSource = Ref<AudioSource>(iaudio->createAudioSource(&source_desc));

		AudioClipDesc clip_desc = {};
		Ref<IO> f = Ref<IO>(fileSystem->openFile(argv[1], IO::READ));
		f->seek(0, IO::SET);
		Ref<AudioDecoder> decoder = Ref<AudioDecoder>(new VorbisAudioDecoder(f));
		printf("bits ! %d\n", decoder->getSampleBitResolution());
		clip_desc.decoder = decoder;
		clip_desc.samples = decoder->getSampleBitResolution();
		clip_desc.sampleRate = decoder->getSampleRate() * decoder->getNrChannels();
		clip_desc.format = AudioFormat::eStero;
		// clip_desc.datamode = AudioDataMode::Streaming;

		Ref<AudioClip> clip = Ref<AudioClip>(iaudio->createAudioClip(&clip_desc));
		printf("Created the audio clip.\n");
		// clip->setData
		audioSource->setClip(*clip);
		printf("Playing the sound.\n");
		audioSource->play();

		usleep(10);
		while (audioSource->isPlaying()) {
			printf("pos sec: %f\n", audioSource->getPos());
			usleep(500);
			audioSource->setVolume(powf(sin(audioSource->getPos()), 2));
		}

		sleep(10);
		delete *iaudio;
	}
	~AudioPlayback(void) {}
};

int main(int argc, const char **argv) {

	try {
		AudioPlayback(argc, argv);
	} catch (fragcore::IException &ex) {
		std::cerr << "Internal exception - " << ex.getName() << std::endl;
		std::cerr << ex.what() << std::endl;
		std::cerr << ex.getBackTrace() << std::endl;
		return EXIT_FAILURE;

	} catch (std::exception &ex) {
		std::cerr << "External library error" << std::endl;
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}