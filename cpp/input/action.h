// Copyright 2015-2015 the openage authors. See copying.md for legal info.

#ifndef OPENAGE_KEYBINDS_ACTION_H_
#define OPENAGE_KEYBINDS_ACTION_H_

#include <vector>

#include "../coord/window.h"
#include "event.h"

namespace openage {
namespace input {

// TODO: use as hint for action arg
enum class action_t {
	UNDEFINED,
	START_GAME,
	STOP_GAME,
	TOGGLE_HUD,
	SCREENSHOT,
	TOGGLE_DEBUG_OVERLAY,
	TOGGLE_DEBUG_GRID,
	QUICK_SAVE,
	QUICK_LOAD,
	TOGGLE_MODE,
	TOGGLE_MENU,
	TOGGLE_ITEM,
	TOGGLE_BLENDING,
	TOGGLE_PROFILER,
	TOGGLE_CONSTRUCT_MODE,
	TOGGLE_UNIT_DEBUG,
	TRAIN_OBJECT,
	ENABLE_BUILDING_PLACEMENT,
	DISABLE_SET_ABILITY,
	SET_ABILITY_MOVE,
	SET_ABILITY_GATHER,
	TOGGLE_CONSOLE,
	SPAWN_VILLAGER,
	KILL_UNIT,
	BUILDING_1,
	BUILDING_2,
	BUILDING_3,
	BUILDING_4,
	BUILDING_TOWN_CENTER,
	SWITCH_TO_PLAYER_1,
	SWITCH_TO_PLAYER_2,
	SWITCH_TO_PLAYER_3,
	SWITCH_TO_PLAYER_4,
	SWITCH_TO_PLAYER_5,
	SWITCH_TO_PLAYER_6,
	SWITCH_TO_PLAYER_7,
	SWITCH_TO_PLAYER_8,
	SELECT,
	DESELECT,
	FORWARD,
	BACK,
	PAINT_TERRAIN
};

struct action_hash {
	int operator()(const action_t& a) const;
};

// TODO: use action_hint_t = std::pair<action_t, int>
// for example a building command
// std::make_pair(action_t::BUILD, 123)
using action_id_t = action_t; //std::pair<action_t, int>;

// TODO use std::vector<action_id_t> to global bindings
// these binds get set globally in input manager
struct action_arg_t {
public:
	action_arg_t(Event e, coord::window m);

	// Triggering event
	const Event e;

	// Mouse position
	const coord::window mouse;

	// hints for arg reciever
	std::vector<action_id_t> hints;
};

using action_func_t = std::function<void(const action_arg_t &)>;

// TODO string description
// This is a base class for event_class, event, action_t mapped to a function
class Action {
public:
	Action(const action_func_t f);
	Action(const action_id_t &type, const action_func_t f);

	/**
	 * pass mouse position
	 */
	bool do_action(const action_arg_t &e);

	/**
	 * type of this action
	 */
	const action_id_t type;

private:
	const action_func_t on_action;

};


} //namespace input
} //namespace openage

#endif
