#pragma once


#include "redactor/Redactor.h"
#define RedactorApp RedactorEngine


class AbstractPlugin {
    // Plugin has many other methods, but what do you whink of those
public:
    AbstractPlugin(RedactorApp *app); // to access tool_manager maybe. but idk

    virtual ~AbstractPlugin();

    virtual void apply();

    // In case we'd like to paint with our plugin using Renderer and Layers (see below). but idk
    virtual void apply(Vec2d pos);
    virtual void apply(Vec2d from, Vec2d to);

    virtual void apply(Layer *layer);
  
    virtual void set_glib_gate(Renderer *renderer); // Renderer can: draw circle, triangle, rectanlge with given positions and colors.

                                                    // Also has .push_target(target) - specifies place to draw everything to,
                                                    // it can be 2d pixle array, sf::Texture or someting similar, spesific for
                                                    // your way to wrap graphics lib functionality. Remember to .pop_target()
                                                    // after pushing it

                                                    // RenderMode (an abstract, that defines BlendingMode and etc.) is super
                                                    // cool, but aren't we too lazy for it?

    virtual void set_layer(Layer *layer); // .get_target() - returnes a target for Renderer to draw everything on
                                          // get_size()

                                          // If you don't have layers, supposingly Canvas hold such functions

    virtual Tool *make_instrument(); // Tool can: on_activate, on_deactivate, on_mouse_down(pos), on_mouse_up(pos),
                                     // on_mouse_move(from, to),
                                     // set_size(double), set_color(color),
                                     // const char *get_name()

                                     // Just in case we want to have minimalistic plugin instruments. but idk
};

