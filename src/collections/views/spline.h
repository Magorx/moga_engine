#pragma once


#include "highlighter.h"
#include "magnetic.h"

#include "utils/interpolator.h"

#include <vector>


const double PX_SPLINE_DOT = 10;
const double PX_SPLINE_MAG_RAD = 10;


class v_Spline : v_Highlighter {
    std::vector<v_Magnetic*> dots;
    std::vector<char> output;

    AppearenceTexture *dot_appearence;

public:
    v_Spline(const ViewBody &body);
    virtual ~v_Spline();
    
    void recalculate_output();

    virtual void render(Renderer *renderer) override;
};
