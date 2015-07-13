// Copyright 2015-2015 the openage authors. See copying.md for legal info.

#include <array>

#include "../log/log.h"
#include "../engine.h"
#include "input_manager.h"

namespace openage {
namespace input {

InputManager::InputManager()
	: // TODO get this from a file instead of hardcoding it here
	keys {{ event_t(SDLK_ESCAPE), action_t::STOP_GAME },
	      { event_t(SDLK_F1), action_t::TOGGLE_HUD },
	      { event_t(SDLK_F2), action_t::SCREENSHOT },
	      { event_t(SDLK_F3), action_t::TOGGLE_DEBUG_OVERLAY },
	      { event_t(SDLK_F4), action_t::TOGGLE_DEBUG_GRID },
	      { event_t(SDLK_F5), action_t::QUICK_SAVE },
	      { event_t(SDLK_F9), action_t::QUICK_LOAD },
	      { event_t(SDLK_SPACE), action_t::TOGGLE_BLENDING },
	      { event_t(SDLK_F12), action_t::TOGGLE_PROFILER },
	      { event_t(SDLK_m), action_t::TOGGLE_CONSTRUCT_MODE },
	      { event_t(SDLK_p), action_t::TOGGLE_UNIT_DEBUG },
	      { event_t(SDLK_t), action_t::TRAIN_OBJECT },
	      { event_t(SDLK_y), action_t::ENABLE_BUILDING_PLACEMENT },
	      { event_t(SDLK_z, KMOD_LCTRL), action_t::DISABLE_SET_ABILITY },
	      { event_t(SDLK_x, KMOD_LCTRL), action_t::SET_ABILITY_MOVE },
	      { event_t(SDLK_c, KMOD_LCTRL), action_t::SET_ABILITY_GATHER },
	      { event_t(SDLK_BACKQUOTE), action_t::TOGGLE_CONSOLE},
	      { event_t(SDLK_v), action_t::SPAWN_VILLAGER},
	      { event_t(SDLK_DELETE), action_t::KILL_UNIT},
	      { event_t(SDLK_q), action_t::BUILDING_1},
	      { event_t(SDLK_w), action_t::BUILDING_2},
	      { event_t(SDLK_e), action_t::BUILDING_3},
	      { event_t(SDLK_r), action_t::BUILDING_4},
	      { event_t(SDLK_q, KMOD_LCTRL), action_t::BUILDING_1},
	      { event_t(SDLK_w, KMOD_LCTRL), action_t::BUILDING_2},
	      { event_t(SDLK_e, KMOD_LCTRL), action_t::BUILDING_3},
	      { event_t(SDLK_r, KMOD_LCTRL), action_t::BUILDING_4},
	      { event_t(SDLK_z), action_t::BUILDING_TOWN_CENTER},
	      { event_t(SDLK_1), action_t::SWITCH_TO_PLAYER_1},
	      { event_t(SDLK_2), action_t::SWITCH_TO_PLAYER_2},
	      { event_t(SDLK_3), action_t::SWITCH_TO_PLAYER_3},
	      { event_t(SDLK_4), action_t::SWITCH_TO_PLAYER_4},
	      { event_t(SDLK_5), action_t::SWITCH_TO_PLAYER_5},
	      { event_t(SDLK_6), action_t::SWITCH_TO_PLAYER_6},
	      { event_t(SDLK_7), action_t::SWITCH_TO_PLAYER_7},
	      { event_t(SDLK_8), action_t::SWITCH_TO_PLAYER_8}},
	keymod{KMOD_NONE} {

}


InputContext &InputManager::get_global_keybind_context() {
	return this->global_hotkeys;
}


void InputManager::override_context(InputContext *context) {
	// Create a new context list, unless no context lists exist
	if (!this->contexts.empty()) {
		this->contexts.push(std::vector<InputContext *>());
	}
	this->contexts.top().push_back(context);
}


void InputManager::register_context(InputContext *context) {
	// Create a context list if none exist
	if (this->contexts.empty()) {
		this->contexts.push(std::vector<InputContext *>());
	}
	this->contexts.top().push_back(context);
}


void InputManager::remove_context() {
	if (this->contexts.empty()) {
		return;
	}
	auto top = this->contexts.top();
	top.pop_back();
	if (top.size() == 0) {
		this->contexts.pop();
	}
}

void InputManager::press(event_t k) {
	log::log(MSG(dbg) << "event");

	// Remove modifiers like num lock and caps lock
	k.mod = static_cast<SDL_Keymod>(k.mod & this->used_keymods);

	// Check whether key combination is bound to an action
	action_t action = action_t::UNDEFINED;
	auto a = this->keys.find(k);
	if (a != this->keys.end()) {
		action = a->second;
	}

	action_arg_t arg;
	arg.e = k;
	arg.mouse = this->mouse_position;

	if (!this->contexts.empty()) {
		// Check context list on top of the stack (most recent bound first)
		for (auto it = this->contexts.top().rbegin(); it != this->contexts.top().rend(); ++it) {
			if ((*it)->execute_if_bound(action, arg)) {
				return;
			}
		}
	}

	// If no local keybinds were bound, check the global keybinds
	this->global_hotkeys.execute_if_bound(action, arg);
}


void InputManager::set_key_state(SDL_Keycode k, SDL_Keymod mod, bool is_down) {
	this->keymod = mod;
	this->key_states[k] = is_down;
}


// not that the function stores a unknown/new keycode
// as 'not pressed' when it was requested
bool InputManager::is_key_down(SDL_Keycode k) {
	auto it = this->key_states.find(k);
	if (it != this->key_states.end()) {
		return it->second;
	} else {
		this->key_states[k] = false;
		return false;
	}
}

bool InputManager::is_keymod_down(SDL_Keymod mod) const {
	return (this->keymod & mod) == mod;
}

bool InputManager::on_input(SDL_Event *e) {
	Engine &engine = Engine::get();

	// top level input handler
	switch (e->type) {

	case SDL_MOUSEBUTTONDOWN: {
		break;
	} // case SDL_MOUSEBUTTONDOWN

	case SDL_MOUSEBUTTONUP: {
		this->press(event_t(event_source_t::MOUSE, 0));
		break;
	} // case SDL_MOUSEBUTTONUP

	case SDL_MOUSEMOTION: {
		this->mouse_position = coord::window {(coord::pixel_t) e->button.x, (coord::pixel_t) e->button.y};
		break;
	} // case SDL_MOUSEMOTION

	case SDL_MOUSEWHEEL: {
		//this->active_mode->on_mouse_wheel(e->wheel.y, mousepos_window);
		break;
	} // case SDL_MOUSEWHEEL

	case SDL_KEYUP: {
		SDL_Keymod keymod = SDL_GetModState();
		SDL_Keycode sym = reinterpret_cast<SDL_KeyboardEvent *>(e)->keysym.sym;
		this->set_key_state(sym, keymod, false);
		this->press(event_t(sym, keymod));
		break;
	}

	case SDL_KEYDOWN: {
		SDL_Keycode sym = reinterpret_cast<SDL_KeyboardEvent *>(e)->keysym.sym;
		this->set_key_state(sym, SDL_GetModState(), true);
		break;
	}

	} // switch (e->type)

	return true;
	return true;
}


} //namespace input
} //namespace openage
