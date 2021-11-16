#include "redactor/engine.h"

#include "app_initialization.h"


int main() {
    srand(time(nullptr));
    logger.set_verb_level(Logger::Level::warning);

    auto window = MogaEngine::create_window("MOGADACTOR", true);

    RedactorEngine moga(window, "MOGADACTOR");
    Resources.init(&moga);

    initialize_photoshop(moga);

    moga.everlasting_loop();

	return 0;
}
