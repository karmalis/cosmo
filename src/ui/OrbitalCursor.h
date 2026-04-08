#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <entt/entt.hpp>

#include "../tactical/Tacview.h"

namespace cosmo::ui {

constexpr float kPixelSizeRatio = 1.0f;
constexpr float kCursorThickness = 0.01f;
constexpr float kCursorSize = 0.46f;
constexpr float kCursorGap = 0.20f;

class OrbitalCursor {
public:
  struct Params {
    float pixel_size_ratio{kPixelSizeRatio};
    float thickness{kCursorThickness};
    float size{kCursorSize};
    float gap{kCursorGap};
  };

  OrbitalCursor() = delete;
  OrbitalCursor(entt::entity entity,
                std::reference_wrapper<tactical::Tacview> tacview);
  void Draw(sf::RenderTarget &target);
  void Update(entt::registry &registry);

  void DisplayControlUi(entt::registry &registry);

  void HandleInput(sf::RenderTarget &target, sf::Event *event,
                   entt::registry &registry);

private:
  sf::Shader shader_;
  sf::Vector2f position_;
  sf::RectangleShape quad_;

  entt::entity entity_;
  std::reference_wrapper<tactical::Tacview> tacview_;

  Params params_;
  bool should_expect_input_;
};

} // namespace cosmo::ui