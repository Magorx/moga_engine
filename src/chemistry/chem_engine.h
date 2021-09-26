#ifndef CHEM_ENGINE_H
#define CHEM_ENGINE_H

#include "MogaEngine.h"

class ChemEngine : public MogaEngine {
    void reactions_process();
	void physics_tick() override final;

public:
    ChemEngine( const char  *window_name,
				const size_t screen_width,
				const size_t screen_height,
				const size_t pixel_size);
};

#endif // CHEM_ENGINE_H
