// Copyright 2013-2015 the openage authors. See copying.md for legal info.

#include "main.h"

#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "args.h"

#include "console/console.h"
#include "log/log.h"
#include "ui/menu.h"
#include "util/error.h"
#include "game_main.h"
#include "game_renderer.h"
#include "testing/testing.h"

#include "config.h"


using namespace openage;

int run_game(Arguments *args) {
	util::Timer timer;

	struct stat data_dir_stat;

	if (stat(args->data_directory, &data_dir_stat) == -1) {
		throw util::Error(MSG(err) <<
			"Failed checking directory " << args->data_directory <<
			": " << strerror(errno));
	}

	log::log(MSG(info) << "launching engine with data directory '" << args->data_directory << "'");

	util::Dir data_dir{args->data_directory};

	timer.start();
	Engine::create(&data_dir, "openage");
	Engine &engine = Engine::get();

	// initialize terminal colors
	auto termcolors = util::read_csv_file<gamedata::palette_color>(data_dir.join("converted/termcolors.docx"));

	console::Console console;
	console.load_colors(termcolors);
	console.register_to_engine(&engine);

	log::log(MSG(info).fmt("Loading time [engine]: %5.3f s", timer.getval() / 1.0e9));

	// init the test run
	timer.start();
	GameRenderer renderer{&engine};
	ui::MainMenu test{&engine};

	log::log(MSG(info).fmt("Loading time   [game]: %5.3f s", timer.getval() / 1.0e9));

	// run main loop
	engine.run();

	Engine::destroy();

	return 0;
}


void init() {
	log::init();
}


void deinit() {
	log::deinit();
}


int main(int argc, char **argv) {
	init();

	try {
		Arguments args = parse_args(argc, argv);

		if (args.error_occured) {
			return 1;
		}

		if (args.display_help) {
			return 0;
		}

		if (args.list_tests) {
			testing::list_tests();
			return 0;
		}

		if (args.version) {
			std::cout << "openage " << config::version << std::endl;
			std::cout << config::config_option_string << std::endl;

			return 0;
		}

		// set global random seed
		srand(time(NULL));

		// shall we run a test?
		if (args.tests.empty() and not args.demo_specified) {
			return run_game(&args);
		}

		bool result = true;
		for (auto &ti : args.tests) {
			if (!testing::run_test(ti)) {
				result = false;
			}
		}

		if (args.demo_specified) {
			if (!testing::run_demo(args.demo, args.demo_argc, args.demo_argv)) {
				result = false;
			};
		}

		return (result == false) ? 1 : 0;

	} catch (util::Error e) {
		log::log(MSG(crit) << "Exception: " << e);
		return 1;
	}

	deinit();
}
