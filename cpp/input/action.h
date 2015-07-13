// Copyright 2015-2015 the openage authors. See copying.md for legal info.

#ifndef OPENAGE_KEYBINDS_ACTION_H_
#define OPENAGE_KEYBINDS_ACTION_H_

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
	SELECT
};

struct action_hash {
	int operator()(const action_t& a) const;
};


// TODO add std::vector<action_t> for train unit global binds
// the binds get set globally in input manager
struct action_arg_t {

	// Triggering event
	event_t e;

	// Mouse position
	coord::window mouse;
};

// TODO std::pair<action_t, int>
using action_id_t = action_t;
using action_func_t = std::function<void(const action_arg_t &)>;

// TODO create int args, string description
// actions should be context specific
class Action {
public:
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
