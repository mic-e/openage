// Copyright 2015-2015 the openage authors. See copying.md for legal info.

#ifndef OPENAGE_KEYBINDS_KEY_H_
#define OPENAGE_KEYBINDS_KEY_H_

#include <unordered_set>

#include <SDL2/SDL.h>

namespace openage {
namespace input {

enum class event_source_t {
	KEYBOARD,
	MOUSE,
	MOUSE_MOTION
};

struct event_source_hash {
	int operator()(const event_source_t &s) const;
};

/**
 * a set sdl event types
 * TODO alternative constructor with char
 */
struct event_t {
public:
	event_t();
	event_t(SDL_Keycode key);
	event_t(SDL_Keycode key, SDL_Keymod mod);
	event_t(event_source_t s, int button);

	// TODO mouse buttons
	event_source_t source;
	SDL_Keycode key;
	SDL_Keymod mod;
};

bool operator ==(event_t a, event_t b);

struct event_hash {
	int operator()(const event_t &k) const;
};

using event_set_t = std::unordered_set<event_t, event_hash>;

} //namespace input
} //namespace openage

#endif
