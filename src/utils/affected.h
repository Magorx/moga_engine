#pragma once


#include <vector>
#include "effect.h"


template <typename T>
class Affected {
    friend Effect<T>;

protected:
    std::vector<Effect<T>*> effects;
    bool effects_applied = false;

public:
    virtual ~Affected() {
        for (auto effect : effects) {
            delete effect;
        }
    }

    virtual void add_effect(Effect<T>* effect) {
        effects.push_back(effect);
    }

    virtual void apply_effects() {
        for (size_t i = 0; i < effects.size(); ++i) {
            effects[i]->apply();
        }
    }

    virtual void apply(Effect<Affected<T>> *effect, bool to_set_target = false) {
        effect->apply_to(this, to_set_target);
    }

    virtual void remove_effect(Effect<T>* effect) {
        for (size_t i = 0; i < effects.size(); ++i) {
            if (effects[i] == effect) {
                for (size_t j = i + 1; j < effects.size(); ++j) {
                    effects[j - 1] = effects[j];
                }
                effects.pop_back();
                break;
            }
        }
    }
};
