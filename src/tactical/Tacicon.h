#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "../orbital/SystemDefinition.h"
#include "Components.h"

namespace cosmo::tactical {

using IconRenderCallback = std::function<void(
    sf::RenderTarget &target, sf::Shader &shader, const sf::Vector2f &position,
    const components::Affiliation &affiliation)>;

class Tacicon {
public:
  Tacicon();

  void DrawContact(sf::RenderTarget &target, const sf::Vector2f &position,
                   components::Affiliation affiliation,
                   components::ContactType contact_type);

  void DrawSystemBody(sf::RenderTarget &target, const sf::Vector2f &position,
                      const double &mass, const double &radius);

  void DrawStar(sf::RenderTarget &target, const sf::Vector2f &position);

private:
  ///
  /// The below depend on
  /// - self type (weapons are hostile towards and potentially dangerous
  /// towards all)
  /// - affiliation (enemies could be variations of rhombus)
  ///
  std::map<components::Affiliation, sf::Color> affiliation_color_atlas_;
  std::map<components::ContactType, IconRenderCallback>
      contact_type_render_map_;
  std::map<components::ContactType, std::unique_ptr<sf::Shader>>
      contact_type_shader_map_;

  sf::RectangleShape quad_;
  sf::Shader system_body_shader_;
  sf::Clock scanline_clock_;

  void RenderShip(sf::RenderTarget &target, sf::Shader &shader,
                  const sf::Vector2f &position,
                  const components::Affiliation &affiliation);

  void RenderMunition(sf::RenderTarget &target, sf::Shader &shader,
                      const sf::Vector2f &position,
                      const components::Affiliation &affiliation);
};

} // namespace cosmo::tactical
