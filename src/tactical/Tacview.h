#ifndef TACVIEW_H
#define TACVIEW_H

#include "../orbital/Components.h"

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <string>
#include <unordered_map>

namespace cosmo::tactical {

class Tacview {
 public:
  struct ViewParams {
    sf::Vector2f focal_point;
    sf::Vector2f screen_center;
    float visual_scale;
    sf::Font& label_font;
  };

  Tacview() = delete;
  Tacview(const ViewParams& params) : view_params_(params) {}

  void Render(entt::registry& registry, sf::RenderTexture& render_texture);
  void DisplayControlUi();
  void UpdateOrbitalLines(entt::registry& registry);

  void Update(entt::registry& registry);

 private:
  ViewParams view_params_;
  bool needs_update_{false};
  std::unordered_map<std::string, sf::VertexArray> orbital_lines_;

  sf::Vector2f GetScreenPosition(const orbital::components::Position& position);
};

}  // namespace cosmo::tactical

#endif