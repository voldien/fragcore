#include "SDLInput.h"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <cstdint>

using namespace fragcore;

void SDLInput::update() noexcept {
	const uint32_t mask = SDL_GetMouseState(&this->x, &this->y);


	this->mouseDown.set(static_cast<size_t>(MouseButton::LEFT_BUTTON),
							(SDL_BUTTON_LMASK & mask) != 0 &&
								!this->mousePressed[static_cast<size_t>(MouseButton::LEFT_BUTTON)]);
	this->mouseDown.set(static_cast<size_t>(MouseButton::RIGHT_BUTTON),
							(SDL_BUTTON_MMASK & mask) != 0 &&
								!this->mousePressed[static_cast<size_t>(MouseButton::RIGHT_BUTTON)]);
	this->mouseDown.set(static_cast<size_t>(MouseButton::MIDDLE_BUTTON),
							(SDL_BUTTON_RMASK & mask) != 0 &&
								!this->mousePressed[static_cast<size_t>(MouseButton::MIDDLE_BUTTON)]);

	this->mouseReleased.set(static_cast<size_t>(MouseButton::LEFT_BUTTON),
							(SDL_BUTTON_LMASK & mask) == 0 &&
								this->mousePressed[static_cast<size_t>(MouseButton::LEFT_BUTTON)]);
	this->mouseReleased.set(static_cast<size_t>(MouseButton::RIGHT_BUTTON),
							(SDL_BUTTON_MMASK & mask) == 0 &&
								this->mousePressed[static_cast<size_t>(MouseButton::RIGHT_BUTTON)]);
	this->mouseReleased.set(static_cast<size_t>(MouseButton::MIDDLE_BUTTON),
							(SDL_BUTTON_RMASK & mask) == 0 &&
								this->mousePressed[static_cast<size_t>(MouseButton::MIDDLE_BUTTON)]);

	this->mousePressed.set(static_cast<size_t>(MouseButton::LEFT_BUTTON), (SDL_BUTTON_LMASK & mask) != 0);
	this->mousePressed.set(static_cast<size_t>(MouseButton::RIGHT_BUTTON), (SDL_BUTTON_MMASK & mask) != 0);
	this->mousePressed.set(static_cast<size_t>(MouseButton::MIDDLE_BUTTON), (SDL_BUTTON_RMASK & mask) != 0);
}

bool SDLInput::anyKey() noexcept { return false; }
// bool SDLInput::getKey() noexcept { return false; }
// bool SDLInput::getKeyPressed() noexcept { return false; }
// bool SDLInput::getKeyReleased() noexcept { return false; }

bool SDLInput::getMousePosition(int *positionX, int *positionY) noexcept {

	return SDL_GetMouseState(positionX, positionY);
}

Vector2 SDLInput::getMouseScroll() const noexcept { return {}; }

bool SDLInput::getMousePressed(const MouseButton button) noexcept {
	return this->mousePressed[static_cast<size_t>(button)];
}
bool SDLInput::getMouseDown(const MouseButton button) noexcept { return this->mouseDown[static_cast<size_t>(button)]; }

bool SDLInput::getMouseReleased(const MouseButton button) noexcept {
	return this->mouseReleased[static_cast<size_t>(button)];
}