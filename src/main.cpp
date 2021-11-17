#include "redactor/engine.h"

#include "app_initialization.h"


int main(int argc, char*[]) {
    srand(time(nullptr));
    logger.set_verb_level(Logger::Level::warning);

    RWindow *window = nullptr;
    
    if (argc > 1) window = MogaEngine::create_window("MOGADACTOR", true);
    else          window = MogaEngine::create_window("MOGADACTOR", {1200, 800});

    window->setActive(true);

    RedactorEngine moga(window, "MOGADACTOR");
    Resources.init(&moga);

    initialize_photoshop(moga);

    moga.everlasting_loop();

    window->setActive(false);

	return 0;
}
