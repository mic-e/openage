// Copyright 2015-2015 the openage authors. See copying.md for legal info.

#include "event.h"

#include <functional>

namespace openage {
namespace input {

event_t::event_t()
	:
	event_t(0) {}


event_t::event_t(SDL_Keycode key)
	:
	event_t(key, KMOD_NONE) {}


event_t::event_t(SDL_Keycode key, SDL_Keymod mod)
	:
	source{event_source_t::KEYBOARD},
	key{key},
	mod{mod} {}


event_t::event_t(event_source_t s, int button)
	:
	source{s},
	key{button},
	mod{KMOD_NONE} {
}


bool operator ==(event_t a, event_t b) {
	return a.key == b.key && a.mod == b.mod;
}

int event_source_hash::operator()(const event_source_t& s) const {
	return std::hash<int>()(static_cast<int>(s));
}

int event_hash::operator()(const event_t &e) const {
	return std::hash<int>()(e.key) ^ event_source_hash()(e.source) * 3664657;
}


} //namespace input
} //namespace openage
