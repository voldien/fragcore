#include "SDLInput.h"
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <cstddef>
#include <cstdint>

using namespace fragcore;

SDLInput::SDLInput() { /*	*/

	int numKeys;
	const Uint8 *state = SDL_GetKeyboardState(&numKeys);

	keyStates.resize(static_cast<size_t>(numKeys * 2));
}

void SDLInput::update() noexcept {
	/*	Extract mouse position.	*/
	const uint32_t mask = SDL_GetMouseState(&this->x, &this->y);

	/*	Update mouse input state.	*/
	{
		this->mouseDown.set(static_cast<size_t>(MouseButton::LEFT_BUTTON),
							(SDL_BUTTON_LMASK & mask) != 0 &&
								!this->mousePressed[static_cast<size_t>(MouseButton::LEFT_BUTTON)]);
		this->mouseDown.set(static_cast<size_t>(MouseButton::RIGHT_BUTTON),
							(SDL_BUTTON_RMASK & mask) != 0 &&
								!this->mousePressed[static_cast<size_t>(MouseButton::RIGHT_BUTTON)]);
		this->mouseDown.set(static_cast<size_t>(MouseButton::MIDDLE_BUTTON),
							(SDL_BUTTON_MMASK & mask) != 0 &&
								!this->mousePressed[static_cast<size_t>(MouseButton::MIDDLE_BUTTON)]);

		this->mouseReleased.set(static_cast<size_t>(MouseButton::LEFT_BUTTON),
								(SDL_BUTTON_LMASK & mask) == 0 &&
									this->mousePressed[static_cast<size_t>(MouseButton::LEFT_BUTTON)]);
		this->mouseReleased.set(static_cast<size_t>(MouseButton::RIGHT_BUTTON),
								(SDL_BUTTON_RMASK & mask) == 0 &&
									this->mousePressed[static_cast<size_t>(MouseButton::RIGHT_BUTTON)]);
		this->mouseReleased.set(static_cast<size_t>(MouseButton::MIDDLE_BUTTON),
								(SDL_BUTTON_MMASK & mask) == 0 &&
									this->mousePressed[static_cast<size_t>(MouseButton::MIDDLE_BUTTON)]);

		/*	*/
		this->mousePressed.set(static_cast<size_t>(MouseButton::LEFT_BUTTON), (SDL_BUTTON_LMASK & mask) != 0);
		this->mousePressed.set(static_cast<size_t>(MouseButton::RIGHT_BUTTON), (SDL_BUTTON_RMASK & mask) != 0);
		this->mousePressed.set(static_cast<size_t>(MouseButton::MIDDLE_BUTTON), (SDL_BUTTON_MMASK & mask) != 0);
	}
}

bool SDLInput::anyKey() noexcept { return false; }

bool SDLInput::getKey(const unsigned int key) {
	const Uint8 *state = SDL_GetKeyboardState(nullptr);

	return state[key];
}

bool SDLInput::getKeyPressed(const unsigned int key_down) {
	const Uint8 *state = SDL_GetKeyboardState(nullptr);

	return state[key_down];
}

bool SDLInput::getKeyReleased(const unsigned int key_released) {
	const Uint8 *state = SDL_GetKeyboardState(nullptr);

	return state[key_released];
}

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

void SDLInput::setMouseWrapAround(bool status) {
#ifdef SDL_HINT_MOUSE_RELATIVE_WARP_MOTION
	SDL_bool confirmation = SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_WARP_MOTION, status ? "1" : "0");
#endif
}
