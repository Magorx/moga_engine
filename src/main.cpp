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
    v_VerticalLayout *vlayout = new v_VerticalLayout({{725, 150}, {250, 250}}, {{0.1, 0.1}, {0.9, 0.9}}, 5, nullptr, col_menu);
    moga.add_view(vlayout);

    // SmartColor *col_submenu = new SmartColor({150, 150, 150});
    v_HorizontalLayout *hlayout = new v_HorizontalLayout({{800, 150}, {100, 300}}, {{0, 0}, {0, 0}}, 5, nullptr, nullptr);
    vlayout->layout_add(hlayout, 2);

    SmartColor *colmc = new SmartColor({50, 50, 50});
    v_MouseCatcher *mc = new v_MouseCatcher({{750, 100}, {200, 40}}, nullptr, colmc);
    vlayout->layout_add(mc);

    // SmartColor *colhl = new SmartColor({100, 100, 100});
    // v_Highlighter *hl1 = new v_Highlighter({{750, 50}, {200, 40}}, colhl);
    // vlayout->layout_add(hl1);

    SmartColor *neon = new SmartColor({30, 235, 235});
    v_Highlighter *hl2 = new v_Highlighter({{0.1, 0}, {0.9, 1}}, colmc, mc);
    hl2->add_label("Options", 15, neon);

    SmartColor *colbutt = new SmartColor({50, 100, 200});
    v_Button *butt = new v_Button({{0, 0}, {0, 0}}, colbutt);
    butt->e_mouse_press.add(new SpawnBallLambda(&moga));
    hlayout->layout_add(butt);

    SmartColor *coltog1 = new SmartColor({100, 100, 100});
    SmartColor *coltog2 = new SmartColor({225, 35, 30});
    v_Toggler *togg = new v_Toggler({{0, 0}, {0, 0}}, coltog1, coltog2);
    hlayout->layout_add(togg);
    
    // // togg->deactivate();
    // togg->e_toggle_activity.add(new HideableActivityToggleAcceptor(togg));
    togg->e_toggle.add(new ToogleChemistryModelLambda(&moga));

    // hl2->e_mouse_press.add(new a_OnPressToggler(hl2, togg));

    SmartColor *pale = new SmartColor({255, 255, 200});
    SmartColor *black = new SmartColor({30, 30, 30});
    v_Menu *menu = new v_Menu({"Fisrt", "Second", "Third", "FOUTH"}, pale, black);
    moga.add_view(menu);

    (*menu)[0]->e_mouse_press.add(new PressChemistryModelToggler(&moga, Event::Activator::State::toggle));

    menu->deactivate();
    menu->e_toggle_activity.add(new HideableActivityToggleAcceptor(menu));
    hl2->e_mouse_move.add(new a_OnHoverToggler(hl2, menu), false);
    hl2->add_subview(menu);
    menu->fit({0, 1}, {1.5, 3});


    v_HorizontalLayout *sublayout = new v_HorizontalLayout({{800, 150}, {100, 300}}, {{0, 0}, {0, 0}}, 0);
    vlayout->layout_add(sublayout, 2);

    SmartColor *colsub = new SmartColor({177, 77, 177});
    v_Submenu *submenu = v_Submenu::Button({{0, 0}, {0, 0}}, colsub, "Engine", {"On", "Off", "Toggle"}, pale, black);
    sublayout->layout_add(submenu);

    (*submenu)[0]->e_mouse_press.add(new PressChemistryModelToggler(&moga, Event::Activator::State::on));
    (*submenu)[1]->e_mouse_press.add(new PressChemistryModelToggler(&moga, Event::Activator::State::off));
    (*submenu)[2]->e_mouse_press.add(new PressChemistryModelToggler(&moga, Event::Activator::State::toggle));

    SmartColor *colsub2 = new SmartColor({177, 77, 177});
    v_Submenu *submenu2 = v_Submenu::Hover({{0, 0}, {0, 0}}, colsub2, "NICE", {"1", "2", "3", "4"}, neon, black);
    // submenu2->set_align({{-0.5, 1}, {0.5, 2}});
    submenu2->set_min_size({20, 0});
    sublayout->layout_add(submenu2);

    

    // v_VerticalLayout *submenu = new v_VerticalLayout({{0, 50}, {100, 100}}, {{0, 0}, {0, 0}}, 0, hl2, pale);
    // submenu->fit({0, 1}, {1.5, 3});

    // submenu->deactivate();
    // submenu->e_toggle_activity.add(new HideableActivityToggleAcceptor(submenu));

    // SmartColor *sch1 = new SmartColor({155, 0, 0});
    // SmartColor *sch2 = new SmartColor({0, 155, 0});
    // SmartColor *sch3 = new SmartColor({0, 155, 155});
    // v_Highlighter *shl1 = new v_Highlighter({{0, 0}, {0, 0}}, sch1);
    // v_Highlighter *shl2 = new v_Highlighter({{0, 0}, {0, 0}}, sch2);
    // v_Highlighter *shl3 = new v_Highlighter({{0, 0}, {0, 0}}, sch3);
    // submenu->layout_add(shl1);
    // submenu->layout_add(shl2);
    // submenu->layout_add(shl3);

    butt->add_label("MORE", 15, neon);

    // hl2->e_mouse_move.add(new a_OnHoverToggler(hl2, submenu), false);


 
    // v_Highlighter *hl3 = new v_Highlighter({{750, 50}, {200, 40}}, colhl);
    // vlayout->layout_add(hl3, 100);

    moga.everlasting_loop();

	return 0;
}
