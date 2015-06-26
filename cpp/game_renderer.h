// Copyright 2015-2015 the openage authors. See copying.md for legal info.

#ifndef OPENAGE_GAME_RENDERER_H_
#define OPENAGE_GAME_RENDERER_H_

#include <SDL2/SDL.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "engine.h"
#include "coord/tile.h"
#include "handlers.h"
#include "keybinds/keybind_manager.h"
#include "player.h"
#include "terrain/terrain.h"
#include "terrain/terrain_object.h"
#include "unit/selection.h"
#include "unit/unit_container.h"
#include "util/externalprofiler.h"
#include "gamedata/gamedata.gen.h"

namespace openage {

/**
 * renders the editor and action views
 */
class GameRenderer :
		openage::InputHandler,
		openage::DrawHandler,
		openage::HudHandler {
public:
	GameRenderer(openage::Engine *e);
	~GameRenderer();

	void move_camera();

	bool on_draw() override;
	bool on_drawhud() override;
	bool on_input(SDL_Event *e) override;

	/**
	 * debug function that draws a simple overlay grid
	 */
	void draw_debug_grid();

	/**
	 * the game this renderer is using
	 */
	GameMain *game() const;

	/**
	 * GameSpec used by this renderer
	 */
	GameSpec *game_spec() const;

	/**
	 * the player point of view which is being rendered
	 */
	Player *player_focus() const;

	// ui components
	UnitSelection selection;
	Texture *gaben;

	// currently selected terrain id
	openage::terrain_t editor_current_terrain;
	int editor_current_building;

	bool debug_grid_active;
	bool clicking_active;
	bool scrolling_active;
	bool dragging_active;
	bool construct_mode;
	bool building_placement;
	bool use_set_ability;
	ability_type ability;

	// mouse position
	coord::camgame mousepos_camgame;
	coord::phys3 mousepos_phys3;
	coord::tile mousepos_tile;

	keybinds::KeybindContext keybind_context;

	util::ExternalProfiler external_profiler;

private:
	openage::Engine *engine;

	/**
	 * decides which type of right mouse click command to issue based on position
	 *
	 * if a unit is at the position the command should target the unit,
	 * otherwise target ground position
	 */
	Command get_action(const coord::phys3 &pos) const;

};

} //namespace openage

#endif
