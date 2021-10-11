#include "chemistry/chem_engine.h"
#include "view/view.h"


const int SCR_W  = 800;
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
    v_Button *butt = new v_Button(ViewBody{{260, 530}, {50, 50}}, col3);
    butt->e_mouse_press.add(new SpawnBallLambda(&moga));
    moga.add_tickable(col3);
    moga.add_view(butt);

    SmartColor *col1 = new SmartColor({100, 100, 100});
    SmartColor *col2 = new SmartColor({225, 35, 30});
    // v_Toggler *togg = new v_Toggler(ViewBody{{200, 530}, {25, 25}}, 
    v_Toggler *togg = new v_Toggler(ViewBody{{12.5, 12.5}, {25, 25}}, 
                                    col1, col2);
    moga.add_tickable(col1);
    moga.add_tickable(col2);
    
    togg->e_toggle.add(new ToogleChemistryModelLambda(&moga));
    togg->e_mouse_press.add(new EventCatcher<Event::MousePress>);
    togg->e_mouse_release.add(new EventCatcher<Event::MouseRelease>);
    // moga.add_view(togg);
    butt->add_subview(togg);

    SmartColor *col4 = new SmartColor({255, 255, 0});
    SmartColor *col5 = new SmartColor({0, 255, 255});
    moga.add_tickable(col4);
    moga.add_tickable(col5);
    butt->add_label("MORE", 15, col5);

    moga.everlasting_loop();

	return 0;
}
