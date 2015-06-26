// Copyright 2015-2015 the openage authors. See copying.md for legal info.

#ifndef OPENAGE_UI_MENU_H_
#define OPENAGE_UI_MENU_H_

#include "../coord/camgame.h"
#include "../engine.h"
#include "../game_main.h"
#include "../handlers.h"

namespace openage {
namespace ui {

using point_t = openage::coord::camgame;

class Element {
public:
	Element(point_t start_point, point_t end_point);

	virtual void render() = 0;

private:
	point_t start, end;

};

/**
 * connects the ui system with the game engine
 */
class MainMenu :
		public Element,
		public openage::HudHandler,
		public openage::InputHandler {
public:
	MainMenu(openage::Engine *engine);

	bool on_drawhud() override;
	bool on_input(SDL_Event *e) override;

	void render() override;

private:

	// access to games asset files
	AssetManager assetmanager;

	game_settings settings;
	Element *root;

};

}} //namespace openage::ui

#endif
