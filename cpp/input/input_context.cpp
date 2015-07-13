// Copyright 2015-2015 the openage authors. See copying.md for legal info.

#include "../log/log.h"
#include "input_context.h"

namespace openage {
namespace input {

InputContext::InputContext() {}

std::vector<std::string> InputContext::active_binds() const {
	std::vector<std::string> result;
	return result;
}

void InputContext::bind(const Action &act) {
	binds.emplace(std::make_pair(act.type, act));
}

void InputContext::bind(const action_t a, const std::function<void()> f) {
	action_func_t af = [f](const action_arg_t &) { f(); };
	binds.emplace(std::make_pair(a, Action(a, af)));
}

void InputContext::set_event(const event_t &e, const action_id_t &type) {
	events.emplace(std::make_pair(e, type));
}

void InputContext::set_event(const event_set_t &es, const action_id_t &type) {
	for (auto &e: es) {
		this->set_event(e, type);
	}
}

bool InputContext::execute_if_bound(const action_t &a, const action_arg_t &arg) {
	action_id_t action_id = a;
	auto event = this->events.find(arg.e);
	if (event != this->events.end()) {
		log::log(MSG(dbg) << "key override");
		action_id = event->second;
	}

	auto f = this->binds.find(action_id);
	if (f != this->binds.end()) {
		f->second.do_action(arg);
		return true;
	}
	return false;
}


} //namespace input
} //namespace openage
