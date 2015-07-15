// Copyright 2015-2015 the openage authors. See copying.md for legal info.

#include "event.h"

#include <functional>

namespace openage {
namespace input {

class_code_t::class_code_t(event_class cl, code_t code)
	:
	eclass(cl),
	code(code) {
}


std::vector<event_class> class_code_t::get_classes() const {
	std::vector<event_class> result;

	// use event_base to traverse up the class tree
	event_class ec = this->eclass;
	result.push_back(ec);
	while (event_base.count(ec) > 0) {
		ec = event_base.at(ec);
		result.push_back(ec);
	}
	return result;
}


bool class_code_t::has_class(const event_class &ec) const {
	for (auto c : this->get_classes()) {
		if (c == ec) {
			return true;
		}
	}
	return false;
}


bool operator ==(class_code_t a, class_code_t b) {
	return a.eclass == b.eclass && a.code == b.code;
}


Event::Event(event_class cl, code_t code, modset_t mod)
	:
	cc(cl, code),
	mod(mod) {}


std::string Event::info() const {
	std::string result;
	result += "[Event: ";
	result += std::to_string(static_cast<int>(this->cc.eclass));
	result += ", ";
	result += std::to_string(this->cc.code);
	result += " (";
	result += std::to_string(this->mod.size());
	result += ")]";
	return result;
}


bool Event::operator ==(const Event &other) const {
	return this->cc == other.cc && this->mod == other.mod;
}


int event_hash::operator()(const Event &e) const {
	return class_code_hash()(e.cc); // ^ std::hash<modset_t>()(e.mod) * 3664657;
}


int event_class_hash::operator()(const event_class& c) const {
	return std::hash<int>()(static_cast<int>(c));
}


int modifier_hash::operator()(const modifier &m) const {
	return std::hash<int>()(static_cast<int>(m));
}


int class_code_hash::operator()(const class_code_t &e) const {
	return event_class_hash()(e.eclass) ^
	       std::hash<int>()(e.code) * 3664657;
}


modset_t sdl_mod(SDL_Keymod) {
	// Remove modifiers like num lock and caps lock
	// mod = static_cast<SDL_Keymod>(mod & this->used_keymods);
	return modset_t();
}


Event sdl_key(SDL_Keycode code) {
	return Event(event_class::KEYBOARD, code, modset_t());
}


Event sdl_key(SDL_Keycode code, SDL_Keymod mod) {
	return Event(event_class::KEYBOARD, code, sdl_mod(mod));
}


Event sdl_mouse(int button) {
	return Event(event_class::MOUSE_BUTTON, button, modset_t());
}


Event sdl_wheel(int direction) {
	return Event(event_class::MOUSE_WHEEL, direction, modset_t());
}


} //namespace input
} //namespace openage
