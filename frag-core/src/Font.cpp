#include"Font.h"
using namespace fragcore;

Font::Font() {
	this->texture = nullptr;
}

Font::~Font() {
	//TODO reduce references.
}

unsigned int Font::getNumberCharacter() const {
	return this->characters.size();
}

Font::Character Font::getCharacter(unsigned int index) {
	return this->characters[index];
}

const Font::Character &Font::getCharacter(unsigned int index) const {
	return this->characters.at(index);
	//return this->characters[index];
}

Texture *Font::getTexture() const {
	return this->texture;
}
