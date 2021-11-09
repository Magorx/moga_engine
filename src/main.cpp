// #define TEST

#ifndef TEST
#include "redactor/engine.h"

#include "app_initialization.h"
#endif

#include "utils/string_input/string_field.h"

int main() {
    #ifndef TEST
    srand(time(nullptr));

    RedactorEngine moga("MOGA", SCR_W, SCR_H, 1);

    Resources.init(&moga);

    initialize_photoshop(moga);

    moga.everlasting_loop();
    #else

    #define D l.dump();

    Line l(2); D

    l.insert_char('a'); D
    l.insert_char('b'); D
    l.insert_char('_'); D
    l.insert_char('c'); D
    l.insert_char('d'); D

    l.insert_char('1'); D
    l.insert_char('2'); D
    l.insert_char('3'); D
    l.insert_char('4'); D

    l.cursor_l(); D
    l.cursor_l(); D
    l.cursor_l(); D
    l.cursor_l(); D
    l.cursor_l(); D
    l.cursor_l(); D


    // l.fix_anchors(); D
    // l.cursor_l(); D
    // l.cursor_l(); D
    // l.cursor_l(); D
    l.erase_next(); D

    printf("---\n[%s]\n", l.c_str());
    #endif

	return 0;
}
