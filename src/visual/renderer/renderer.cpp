#include "renderer.h"


Renderer::Renderer(const char *window_name, int size_x, int size_y):
scr(new sf::RenderWindow(sf::VideoMode(size_x, size_y), window_name), size_x, size_y)
{}
