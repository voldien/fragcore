#include "Core/IO/IO.h"
#include "decoder/VorbisDecoder.h"
#include <ogg/ogg.h>
#include <opus/opus.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
using namespace fragcore;

// TODO resolve.
struct ogg_file {
	Ref<IO> io;
};

size_t AR_readOgg(void *dst, size_t size1, size_t size2, void *fh) {
	ogg_file *of = reinterpret_cast<ogg_file *>(fh);
	return of->io->read(size1 * size2, dst);
}

int AR_seekOgg(void *fh, ogg_int64_t to, int type) {
	ogg_file *of = reinterpret_cast<ogg_file *>(fh);

	IO::Seek seek;
	switch (type) {
	case SEEK_CUR:
		seek = IO::CUR;
		break;
	case SEEK_END:
		seek = IO::END;
		break;
	case SEEK_SET:
		seek = IO::SET;
		break;
	default:
		return -1;
	}
	of->io->seek(to, seek);
	return 0;
}

int AR_closeOgg(void *fh) {
	ogg_file *of = reinterpret_cast<ogg_file *>(fh);
	// of->io->deincrement(;)
	return 0;
}

long AR_tellOgg(void *fh) {
	ogg_file *of = reinterpret_cast<ogg_file *>(fh);
	return of->io->getPos();
}

VorbisAudioDecoder::VorbisAudioDecoder(Ref<IO> &io) : AudioDecoder(io) {
	ov_callbacks callbacks;
	ogg_file *t = new ogg_file();
	t->io = io;
	// t.fileSize = io->length();

	/*	Required IO operation to be supported.	*/
	const IO::IOOperation requiredIOReadSupported =
		static_cast<IO::IOOperation>(IO::OP_READ | IO::OP_SEEK | IO::OP_GETPOS);
	if (!io->isOperationSupported(requiredIOReadSupported)) {
		// Must support the following commands.
	}

	ov = new OggVorbis_File;
	// mOggFile = ov;
	memset(ov, 0, sizeof(OggVorbis_File));

	/*	*/
	callbacks.read_func = AR_readOgg;
	callbacks.seek_func = AR_seekOgg;
	callbacks.close_func = AR_closeOgg;
	callbacks.tell_func = AR_tellOgg;

	io->seek(0, IO::SET);
	int ret = ov_open_callbacks((void *)t, ov, nullptr, 0, callbacks);

	if (ov_seekable(ov)) {
		// TODO remove
		printf("Input bitstream contained %ld logical bitstream section(s).\n", ov_streams(ov));
		printf("Total bitstream playing time: %ld seconds\n\n", (long)ov_time_total(ov, -1));
		this->length = ov_time_total(ov, -1);
	}

	vorbis_info *vi = ov_info(ov, -1);
	vorbis_comment *comment = ov_comment(ov, 0);
	vorbis_info *info = ov_info(ov, 0);
	printf("{}\n", info->channels);
	assert(vi);

	for (int i = 0; i < ov_streams(ov); i++) {
		vorbis_info *vi = ov_info(ov, i);
		// TODO remove

		printf("\tlogical bitstream section %d information:\n", i + 1);
		printf("\t\t%ldHz %d channels bitrate %ldkbps serial number=%ld\n", vi->rate, vi->channels,
			   ov_bitrate(ov, i) / 1000, ov_serialnumber(ov, i));
		fprintf(stderr, "\nDecoded length: %ld samples\n", (long)ov_pcm_total(ov, -1));
		printf("\t\tcompressed length: %ld bytes ", (long)(ov_raw_total(ov, i)));
		printf("play time: %lds\n", (long)ov_time_total(ov, i));

		this->channels = vi->channels;
		this->sampleFreq = vi->rate;
		this->bitRate = (long)ov_bitrate(ov, -1);
	}
}

VorbisAudioDecoder::~VorbisAudioDecoder() { ov_clear(ov); }

void VorbisAudioDecoder::seek(long int microseconds) { ov_time_seek(ov, microseconds * 1000.0); }

void *VorbisAudioDecoder::getData(long int *size) {
	vorbis_info *vi = ov_info(ov, -1);
	size_t data_len = ov_pcm_total(ov, -1) * vi->channels * 4;

	void *pcmout1 = malloc(data_len);
	if (pcmout1 == 0)
		throw RuntimeException("Out of memory.\n");

	// fill pcmout buffer with ov_read data samples
	// you can't just slap data_len in place of 4096, it doesn't work that way
	// 0 is endianess, 0 for little, 1 for big
	// 2 is the data type short's size, mine is 2
	// 1 is the signedness you want, I want short not unsigned short (for openal) so 1
	for (long size = 0, offset = 0, sel = 0;
		 (size = ov_read(ov, (char *)pcmout1 + offset, 4096, 0, 2, 1, (int *)&sel)) != 0; offset += size) {
		if (size < 0)
			throw RuntimeException("");
	}

	*size = data_len;
	return pcmout1;
}
char pcmout[4096 * 16];

void *VorbisAudioDecoder::getPage(int i) {

	int current_section;
	int eof = 0;
	while (!eof) {
		//{
		// ov_pcm_seek(ov, 0);

		long ret = ov_read(ov, pcmout, 1024, 0, 2, 1, &current_section);
		if (ret == 0) {
			/* EOF */
			eof = 1;
		} else if (ret < 0) {

			/* error in the stream.  Not a problem, just reporting it in
		case we (the app) cares.  In this case, we don't. */
		} else {
			/* we don't bother dealing with sample rate changes, etc, but
	you'll have to*/
			fwrite(pcmout, 1, ret, stdout);
			break;
		}
	}
	return pcmout;
	// ov_read
}

AudioFormat VorbisAudioDecoder::getFormat() const {
	switch (this->channels) {
	case 1:
		return AudioFormat::eMono;
	default:
		break;
	}
	return AudioFormat::eMono;
}
unsigned int VorbisAudioDecoder::getSampleRate() const { return sampleFreq; }

unsigned int VorbisAudioDecoder::getNrChannels() const { return channels; }
unsigned int VorbisAudioDecoder::getSampleBitResolution() const {
	float p = (float)this->bitRate / (float)(getSampleRate() * getNrChannels());
	return std::ceil(p) * 8;
}

double VorbisAudioDecoder::getTotalTime() const { return this->length; }