#include "slider.h"


template <typename T>
class v_Slider : public View {
    SliderRange range;
    T &val;

    T (law)(const T &value, float factor);

public:
    v_Slider(const ViewBody &body, const SliderRange &range, T (law)(const T &value, float factor), 
             SmartColor *bar_color, SmartColor *point_color);

    virtual void tick(const double = 0, const double = 0) override;

    void bind(MouseLambda *on_click);

    virtual void clicked(Vec2d click) override;
    virtual void hovered(Vec2d from, Vec2d to) override;
    virtual void released(Vec2d click) override;
};
