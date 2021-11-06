#pragma once

#include <SFML/Graphics.hpp>

#include "utils/vec2d.h"
#include "visual/color/color.h"

using RMode = sf::RenderStates;
using RVertex = sf::Vertex;
using RColor = RGBA;
using RTexture = sf::Texture;
using RRendTexture = sf::RenderTexture;
using RFont = sf::Font;
using RState = sf::RenderStates;
using RShader = sf::Shader;
using RImage = sf::Image;

using RBlendMode = sf::BlendMode;

namespace RBlend {
    const RBlendMode none = sf::BlendNone;
    const RBlendMode add = sf::BlendAdd;
    const RBlendMode alpha = sf::BlendAlpha;
    const RBlendMode multiply = sf::BlendMultiply;
}