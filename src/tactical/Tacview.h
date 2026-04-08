#ifndef TACVIEW_H
#define TACVIEW_H

#include "../orbital/Components.h"
#include "Components.h"
#include "Tacicon.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <entt/entt.hpp>
#include <string>
#include <unordered_map>

namespace cosmo::tactical {

class Tacview {
public:
  struct ViewParams {
    orbital::components::Position focal_point;
    sf::Vector2f screen_center;
    float visual_scale;
    std::reference_wrapper<sf::Font> label_font;
  };

  Tacview() = delete;
  Tacview(const ViewParams &params) : view_params_(params) {}

  void Render(entt::registry &registry, sf::RenderTexture &render_texture);
  void DisplayControlUi();
  void UpdateOrbitalLines(entt::registry &registry);

  void Update(entt::registry &registry);

  sf::Vector2f GetScreenPosition(const orbital::components::Position &position);
  orbital::components::Position
  GetPositionFromScreen(const sf::Vector2f &position);

private:
  ViewParams view_params_;
  bool needs_update_{false};
  std::unordered_map<std::string, sf::VertexArray> orbital_lines_;
  Tacicon tacicon_;

  void RenderIcon(sf::RenderTexture &render_texture, const sf::Vector2f &pos,
                  const components::Affiliation affiliation,
                  const components::ContactType type);
};

} // namespace cosmo::tactical

#endif