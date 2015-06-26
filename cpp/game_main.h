// Copyright 2014-2015 the openage authors. See copying.md for legal info.

#ifndef OPENAGE_GAME_MAIN_H_
#define OPENAGE_GAME_MAIN_H_

#include <SDL2/SDL.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "args.h"
#include "assetmanager.h"
#include "datamanager.h"
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

class Unit;

/**
 * runs the game.
 */
int run_game(openage::Arguments *args);

void gametest_init(openage::Engine *engine);
void gametest_destroy();

/**
 * Contains information for a single game
 * TODO: include a list of actions to be saved
 *       as the game replay file
 */
class GameMain {
public:
	GameMain(const game_settings &sets);
	~GameMain();

	game_settings *get_settings();

	void update();

	/**
	 * map information
	 */
	std::shared_ptr<Terrain> terrain;

	/**
	 * all players in the game
	 */
	std::vector<Player> players;

	/**
	 * all the objects that have been placed.
	 */
	UnitContainer placed_units;

private:
	game_settings settings;

};

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
	 * data manager used by this renderer
	 */
	DataManager *datamanager() const;

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
