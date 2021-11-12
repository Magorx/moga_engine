#include "redactor/engine.h"

#include "app_initialization.h"


int main() {
    srand(time(nullptr));
    logger.set_verb_level(Logger::Level::warning);

    RedactorEngine moga("MOGA", SCR_W, SCR_H, 1);
    Resources.init(&moga);

    initialize_photoshop(moga);

    moga.everlasting_loop();

	return 0;
}
