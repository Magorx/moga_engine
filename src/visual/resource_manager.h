#pragma once


#include <vector>

#include "color/smart_color.h"
#include "renderer/appearence.h"


class VisualResourceManager {
    std::vector<RTexture*> appearence;
    std::vector<SmartColor*> smart_color;

public:
    VisualResourceManager &operator=(const VisualResourceManager &other) = delete; 

    ~VisualResourceManager() {
        for (auto res : appearence) {
            delete res;
        }

        for (auto res : smart_color) {
            delete res;
        }
    }

    inline void add(RTexture* res) { appearence.push_back(res); }
    inline void add(SmartColor* res) { smart_color.push_back(res); }
};
