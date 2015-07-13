// Copyright 2015-2015 the openage authors. See copying.md for legal info.

#ifndef OPENAGE_KEYBINDS_KEYBIND_CONTEXT_H_
#define OPENAGE_KEYBINDS_KEYBIND_CONTEXT_H_

#include <functional>
#include <stack>
#include <unordered_map>
#include <vector>

#include "action.h"
#include "event.h"
#include "../handlers.h"

namespace openage {
namespace input {

class InputContext {

public:
	InputContext();

	/**
	 * a list of all keys which are bound in the current context
	 */
	std::vector<std::string> active_binds() const;

	void bind(const Action &act);

	/**
	 * bind a function to an action.
	 * TODO: remove this function
	 */
	void bind(const action_t a, const std::function<void()> f);

	/**
	 * addtional events to respond to
	 */
	void set_event(const event_t &e, const action_id_t &type);

	/**
	 * connect a set of events to an action
	 */
	void set_event(const event_set_t &es, const action_id_t &type);

	/**
	 * lookup an action. If it is bound, execute it.
	 * @return true when the action is executed, false else.
	 */
	bool execute_if_bound(const action_t &a, const action_arg_t &e);
private:

	/**
	 * TODO: adds context specific tags to event arg
	 */
	std::unordered_map<event_t, action_id_t, event_hash> events;

	/**
	 * action to function map
	 */
	std::unordered_map<action_id_t, Action, action_hash> binds;
};

} //namespace input
} //namespace openage

#endif
