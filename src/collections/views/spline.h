#pragma once


#include "highlighter.h"
#include "magnetic.h"

#include "utils/interpolator.h"

#include <vector>


const double PX_SPLINE_DOT = 10;
const double PX_SPLINE_MAG_RAD = 10;


class SplineDotChangeAcceptor;
class SplineSpawnNewDot;
class SplineUncaptureDot;


class v_Spline : public v_Highlighter {
    friend SplineDotChangeAcceptor;
    friend SplineSpawnNewDot;
    friend SplineUncaptureDot;

    std::vector<v_Magnetic*> dots;
    std::vector<int> output;

    AppearenceTexture *dot_appr;

    bool dot_captured = false;

public:
    v_Spline(const ViewBody &body);
    virtual ~v_Spline();
    
    void recalculate_output();

    virtual void render(Renderer *renderer) override;
};

class SplineSpawnNewDot : public EventAcceptor<v_Spline, Event::MousePress> {
    friend v_Spline;
public:
    SplineSpawnNewDot(v_Spline *spline);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class SplineUncaptureDot : public EventAcceptor<v_Spline, Event::MouseRelease> {
    friend v_Spline;
public:
    SplineUncaptureDot(v_Spline *spline);

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};

class SplineDotChangeAcceptor : public EventAcceptor<v_Spline, Event::FractionChanged> {
    friend v_Spline;
public:
    SplineDotChangeAcceptor(v_Spline *spline);

    EventAccResult operator()(const Event::FractionChanged &event, const EventAccResult *cur_res = nullptr) override;
};

