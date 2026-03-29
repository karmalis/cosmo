//
// Created by voltairepunk on 3/22/26.
//

#ifndef COSMO_BODYRENDERER_H
#define COSMO_BODYRENDERER_H

#include <SFML/Graphics.hpp>

#include "Components.h"

namespace cosmo::orbital {
class BodyRenderer {
 public:

    struct RenderParams {
        components::Position focal_point;
        sf::Vector2f screen_center;
        double visual_scale;
        sf::Font& font;
    };

  static void Render(std::string name, sf::RenderTexture& render_texture,
                     const components::Position& position, const RenderParams& params);
};
}  // namespace cosmo::orbital

#endif  // COSMO_BODYRENDERER_H
