#include "chemistry/chem_engine.h"


#include "app_initialization.h"


int main() {
    Resources.init();

    srand(time(nullptr));
    ChemEngine moga("MOGA", SCR_W, SCR_H, 1);

    initialize_photoshop(moga);

    moga.everlasting_loop();

	return 0;
}
