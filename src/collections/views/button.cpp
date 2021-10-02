#include "button.h"


v_Button::v_Button(const ViewBody &body, SmartColor *color, Lambda *reaction, char *lable):
View(body, new r_Rectangle(body.position, body.size, color)),
lable(lable)
{}

void v_Button::bind(Lambda *reaction) {
    set_reaction(reaction);
}
