#pragma once


#include "visual/animation/animation.h"
#include "texture.h"


class AppearenceAnimation : public AbstractAnimation, public AppearenceTexture {
    const std::vector<RTexture*> *frames;
    double frame_duration;

public:
    AppearenceAnimation(const std::vector<RTexture*> *frames, double frame_duration, bool looped = true, double time_coef = 1) :
    AbstractAnimation(frames ? frame_duration * frames->size() : 0, looped, time_coef),
    frames(frames)
    {}

    virtual void tick(const double dt = 0, const double absolute_time = 0) override { 
        AbstractAnimation::tick(dt, absolute_time);

        if (is_running()) {
            set_texture((*frames)[(size_t) (elapsed() / frame_duration)]);
        }
    };
};
