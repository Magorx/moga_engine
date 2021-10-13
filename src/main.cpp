#include "chemistry/chem_engine.h"


const int SCR_W  = 1000;
const int SCR_H  = 600;


#include "scene_generation.h"
#include "responses.h"


int main() {
    srand(time(NULL));
    ChemEngine moga("MOGA", SCR_W, SCR_H, 1);

    SmartColor *color = new SmartColorSin(Color{40, 230, 150});
    moga.add_tickable(color);

    create_cage(&moga, color);
    generate_balls(&moga, 5);

    SmartColor *col3 = new SmartColor({100, 100, 230});
    v_Button *butt = new v_Button(ViewBody{{800, 400}, {100, 100}}, col3);
    butt->e_mouse_press.add(new SpawnBallLambda(&moga));
    moga.add_tickable(col3);
    moga.add_view(butt);

    SmartColor *col1 = new SmartColor({100, 100, 100});
    SmartColor *col2 = new SmartColor({225, 35, 30});
    v_Toggler *togg = new v_Toggler(ViewBody{{0.1, 0.1}, {0.1, 0.6}},
                                    col1, col2, butt);
    moga.add_tickable(col1);
    moga.add_tickable(col2);

    v_Highlighter *hl = new v_Highlighter({{750, 50}, {200, 40}}, col1);
    moga.add_view(hl);
    
    togg->e_toggle.add(new ToogleChemistryModelLambda(&moga));
    togg->e_mouse_press.add(new EventCatcher<Event::MousePress>);
    togg->e_mouse_release.add(new EventCatcher<Event::MouseRelease>);

    SmartColor *col4 = new SmartColor({120, 120, 100});
    SmartColor *col5 = new SmartColor({0, 255, 255});
    moga.add_tickable(col4);
    moga.add_tickable(col5);
    butt->add_label("MORE", 15, col5, col4);

    moga.everlasting_loop();

	return 0;
}
