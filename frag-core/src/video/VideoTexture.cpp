#include"Video/VideoTexture.h"
#include"Video/VideoManager.h"
#include"Audio/decoder/AudioDecoder.h"
#include"Core/Ref.h"
using namespace fragcore;

VideoTexture::VideoTexture(void) {
	//VideoManager::getInstance()->registerVideo(Ref<VideoTexture>(this));
}

VideoTexture::~VideoTexture(void) {
	//VideoManager::getInstance()->unRegisterVideo(Ref<VideoTexture>(this));
}

bool VideoTexture::isPlaying(void) const{

}

void VideoTexture::play(void) {
	
}

void VideoTexture::stop(void) {

}

void VideoTexture::pause(void) {

}

float VideoTexture::getCurrentPosition(void) const {

}

float VideoTexture::getTotalLength(void) const {

}

double VideoTexture::getDuration(void) const {

}

double VideoTexture::getTime(void) const {

}

bool VideoTexture::isReadyToPlay(void) const {

}

Texture *VideoTexture::getTexture(void) const {
	return (Texture*)this->texture.ptr();
}