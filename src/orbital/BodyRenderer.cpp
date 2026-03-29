//
// Created by voltairepunk on 3/22/26.
//

#include "BodyRenderer.h"

namespace cosmo::orbital {

constexpr double kZeroDivGuard = 0.00001;
constexpr double kLogCompression = 0.000001;

void BodyRenderer::Render(std::string name, sf::RenderTexture& render_texture,
                          const components::Position& position, const RenderParams& params) {
  const auto rel_x = position.x - params.focal_point.x;
  const auto rel_y = position.y - params.focal_point.y;

  const double distance = std::sqrt(rel_x * rel_x + rel_y * rel_y);

  float screen_x, screen_y;

  if (distance < kZeroDivGuard) {
    screen_x = params.screen_center.x;
    screen_y = params.screen_center.y;
  } else {
    const auto log_distance =
        params.visual_scale * std::log10((kLogCompression + distance) + 1.0);
    const auto dir_x = rel_x / distance;
    const auto dir_y = rel_y / distance;

    screen_x = params.screen_center.x + static_cast<float>(dir_x * log_distance);
    screen_y = params.screen_center.y - static_cast<float>(dir_y * log_distance);
  }

  sf::CircleShape circle;
  circle.setFillColor(sf::Color::White);
  circle.setOutlineColor(sf::Color::Green);
  circle.setRadius(5);
  circle.setOutlineThickness(1.0);
  circle.setOrigin({0, 0});
  circle.setPosition(sf::Vector2f{screen_x, screen_y});

  sf::Text body_label(params.font);
  body_label.setString(name);
  body_label.setFillColor(sf::Color::Red);
  body_label.setCharacterSize(16);
  body_label.setPosition(sf::Vector2f{screen_x + 10, screen_y + 10});

  render_texture.draw(circle);
  render_texture.draw(body_label);
}
}  // namespace cosmo::orbital
