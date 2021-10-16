#include "chemistry/chem_engine.h"


const int SCR_W  = 1000;
const int SCR_H  = 600;


#include "scene_generation.h"
#include "responses.h"


int main() {
    srand(time(nullptr));
    ChemEngine moga("MOGA", SCR_W, SCR_H, 1);

    SmartColor *color = new SmartColorSin(Color{40, 230, 150});
    moga.add_tickable(color);

    create_cage(&moga, color);
    generate_balls(&moga, 10);

    SmartColor *col_menu = new SmartColor({200, 200, 200});
    v_VerticalLayout *vlayout = new v_VerticalLayout({{750, 150}, {200, 250}}, {{0.1, 0.1}, {0.1, 0.1}}, 5, nullptr, col_menu);
    moga.add_view(vlayout);

    SmartColor *col_submenu = new SmartColor({150, 150, 150});
    v_HorizontalLayout *hlayout = new v_HorizontalLayout({{800, 150}, {100, 300}}, {{0, 0.0}, {0, 0.0}}, 5, nullptr, nullptr);
    vlayout->layout_add(hlayout, 2);

    SmartColor *colmc = new SmartColor({50, 50, 50});
    v_MouseCatcher *mc = new v_MouseCatcher({{750, 100}, {200, 40}}, nullptr, colmc);
    vlayout->layout_add(mc);

    SmartColor *colhl = new SmartColor({100, 100, 100});
    v_Highlighter *hl1 = new v_Highlighter({{750, 50}, {200, 40}}, colhl);
    vlayout->layout_add(hl1);

    SmartColor *neon = new SmartColor({30, 235, 235});
    v_Highlighter *hl2 = new v_Highlighter({{0.1, 0}, {0.1, 0}}, colmc, mc);
    hl2->add_label("Options", 15, neon);

    SmartColor *colbutt = new SmartColor({50, 100, 200});
    v_Button *butt = new v_Button({{0, 0}, {0, 0}}, colbutt);
    butt->e_mouse_press.add(new SpawnBallLambda(&moga));
    hlayout->layout_add(butt);

    SmartColor *coltog1 = new SmartColor({100, 100, 100});
    SmartColor *coltog2 = new SmartColor({225, 35, 30});
    v_Toggler *togg  = new v_Toggler({{0, 0}, {0, 0}}, coltog1, coltog2);
    hlayout->layout_add(togg);
    
    togg->deactivate();
    togg->e_toggle.add(new ToogleChemistryModelLambda(&moga));

    hl2->e_mouse_press.add(new a_OnPressToggler(hl2, togg));

    butt->add_label("MORE", 15, neon);

    // v_Highlighter *hl3 = new v_Highlighter({{750, 50}, {200, 40}}, colhl);
    // vlayout->layout_add(hl3, 100);

    moga.everlasting_loop();

	return 0;
}
