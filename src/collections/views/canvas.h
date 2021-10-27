#pragma once


#include "labeled_view.h"
#include "redactor/canvas.h"


class v_Canvas : public AbstractLabledView {
    Canvas *canvas;

public:
    v_Canvas(const ViewBody &body) :
    AbstractLabledView(body)
    {
        
    }
};
