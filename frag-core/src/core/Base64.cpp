#include "Core/Base64.h"
#include "Core/IO/IOUtil.h"

using namespace fragcore;

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
										"abcdefghijklmnopqrstuvwxyz"
										"0123456789+/";

void Base64::encode(Ref<IO> &input, Ref<IO> &encoded) { /*	*/
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];

	/*	*/
	input->readable();
	encoded->writeable();
}
void Base64::decode(Ref<IO> &encoded, Ref<IO> &output) { /*	*/
}
