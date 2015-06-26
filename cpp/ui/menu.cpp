// Copyright 2015-2015 the openage authors. See copying.md for legal info.

#include "menu.h"

namespace openage {
namespace ui {

Element::Element(point_t start_point, point_t end_point) {
	this->start = start_point;
	this->end = end_point;
}

MainMenu::MainMenu(openage::Engine *engine)
	:
	Element{point_t{0, 0}, point_t{200, 200}},
	root{nullptr} {

	// add handlers
	engine->register_drawhud_action(this);
	engine->register_input_action(this);

	this->settings.assetmanager = std::make_shared<AssetManager>(engine->get_data_dir());
	this->settings.datamanager = std::make_shared<DataManager>();

	// prepare data loading
	this->settings.datamanager->initialize(this->settings.assetmanager.get());

}

bool MainMenu::on_drawhud() {

	// TODO: move these somewhere else
	this->settings.assetmanager->check_updates();
	this->settings.datamanager->check_updates();

	this->render();
	return true;
}

bool MainMenu::on_input(SDL_Event *e) {
	Engine &engine = Engine::get();
	switch (e->type) {
		case SDL_QUIT:
			engine.stop();
			break;

		case SDL_MOUSEBUTTONDOWN:
			engine.start_game(this->settings);
			break;

	}
	return true;
}

void MainMenu::render() {
	if (root) {
		root->render();
	}
}

}} //namespace openage::ui