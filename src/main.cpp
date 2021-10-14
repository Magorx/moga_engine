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
    v_VerticalLayout *vlayout = new v_VerticalLayout({{800, 150}, {100, 300}}, {{0.1, 0.1}, {0.1, 0.1}}, 5, nullptr, col_menu);
    moga.add_view(vlayout);

    SmartColor *col_submenu = new SmartColor({150, 150, 150});
    v_VerticalLayout *hlayout = new v_VerticalLayout({{800, 150}, {100, 300}}, {{0.1, 0}, {0.1, 0}}, 5, nullptr, col_submenu);
    vlayout->layout_add(hlayout, 2);

    SmartColor *colmc = new SmartColor({50, 50, 50});
    v_MouseCatcher *mc = new v_MouseCatcher({{750, 100}, {200, 40}}, nullptr, colmc);
    vlayout->layout_add(mc);

    SmartColor *colhl = new SmartColor({100, 100, 100});
    v_Highlighter *hl1 = new v_Highlighter({{750, 50}, {200, 40}}, colhl);
    vlayout->layout_add(hl1);

    SmartColor *neon = new SmartColor({30, 235, 235});
    v_Highlighter *hl2 = new v_Highlighter({{0.1, 0.1}, {0.5, 0.1}}, colmc, mc);
    hl2->add_label("Options", 15, neon);

    SmartColor *colbutt = new SmartColor({100, 100, 230});
    v_Button *butt = new v_Button({{0, 0}, {0, 0}}, colbutt);
    butt->e_mouse_press.add(new SpawnBallLambda(&moga));
    hlayout->layout_add(butt);

    SmartColor *coltog1 = new SmartColor({100, 100, 100});
    SmartColor *coltog2 = new SmartColor({225, 35, 30});
    v_Toggler *togg  = new v_Toggler({{0, 0}, {0, 0}}, coltog1, coltog2);
    hlayout->layout_add(togg);
    
    togg->e_toggle.add(new ToogleChemistryModelLambda(&moga));
    togg->e_mouse_press.add(new EventCatcher<Event::MousePress>);
    togg->e_mouse_release.add(new EventCatcher<Event::MouseRelease>);

    butt->add_label("MORE", 15, neon);

    moga.everlasting_loop();

	return 0;
}
