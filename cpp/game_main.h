// Copyright 2014-2015 the openage authors. See copying.md for legal info.

#ifndef OPENAGE_GAME_MAIN_H_
#define OPENAGE_GAME_MAIN_H_

#include <memory>
#include <vector>

#include "engine.h"
#include "player.h"
#include "terrain/terrain.h"
#include "unit/unit_container.h"

namespace openage {

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

} //namespace openage

#endif
