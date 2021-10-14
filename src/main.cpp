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

    SmartColor *col_menu = new SmartColor({200, 200, 200});
    v_HorizontalLayout *layout = new v_HorizontalLayout({{800, 150}, {100, 300}}, {{0.1, 0}, {0.1, 0}}, 20, nullptr, col_menu);
    moga.add_view(layout);

    SmartColor *col3 = new SmartColor({100, 100, 230});
    v_Button *butt = new v_Button({{0, 0}, {0, 0}}, col3);
    butt->e_mouse_press.add(new SpawnBallLambda(&moga));
    moga.add_tickable(col3);
    // moga.add_view(butt);
    layout->layout_add(butt, 5);

    SmartColor *col1 = new SmartColor({100, 100, 100});
    SmartColor *col2 = new SmartColor({225, 35, 30});

    SmartColor *colmc = new SmartColor({50, 50, 50});
    v_MouseCatcher *mc = new v_MouseCatcher({{750, 100}, {200, 40}}, nullptr, colmc);
    // moga.add_view(mc);
    layout->layout_add(mc);

    v_Highlighter *hl1 = new v_Highlighter({{750, 50}, {200, 40}}, col1);
    // moga.add_view(hl1);
    layout->layout_add(hl1);

    v_Highlighter *hl2 = new v_Highlighter({{0.1, 0.1}, {0.5, 0.1}}, colmc, mc);

    v_Toggler *togg  = new v_Toggler({{0.1, 0.1}, {0.1, 0.6}}, col1, col2, butt);
    moga.add_tickable(col1);
    moga.add_tickable(col2);
    
    togg->e_toggle.add(new ToogleChemistryModelLambda(&moga));
    togg->e_mouse_press.add(new EventCatcher<Event::MousePress>);
    togg->e_mouse_release.add(new EventCatcher<Event::MouseRelease>);

    SmartColor *col4 = new SmartColor({120, 120, 100});
    SmartColor *col5 = new SmartColor({0, 255, 255});
    moga.add_tickable(col4);
    moga.add_tickable(col5);

    butt->add_label("MORE", 15, col5);
    hl2->add_label("Options", 15, col5);

    moga.everlasting_loop();

	return 0;
}
