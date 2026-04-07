#include "Tacicon.h"
#include "Components.h"
#include <SFML/System/Angle.hpp>

namespace cosmo::tactical {

Tacicon::Tacicon() {
  contact_type_shader_map_.emplace(components::ContactType::kShip,
                                   std::make_unique<sf::Shader>());
  if (auto &shader = contact_type_shader_map_[components::ContactType::kShip];
      !shader->loadFromFile("data/shaders/tacicon1.vert",
                            "data/shaders/tactical/contact/ship.frag")) {
    // TODO: Logging
  }
  contact_type_shader_map_.emplace(components::ContactType::kMunition,
                                   std::make_unique<sf::Shader>());
  if (auto &shader =
          contact_type_shader_map_[components::ContactType::kMunition];
      !shader->loadFromFile("data/shaders/tacicon1.vert",
                            "data/shaders/tactical/contact/munition.frag")) {
    // TODO: Logging
  }

  contact_type_render_map_.emplace(
      components::ContactType::kShip,
      [this](auto &target, auto &shader, const auto &position,
             const auto &color) {
        RenderShip(target, shader, position, color);
      });
  contact_type_render_map_.emplace(
      components::ContactType::kMunition,
      [this](auto &target, auto &shader, const auto &position,
             const auto &color) {
        RenderMunition(target, shader, position, color);
      });

  affiliation_color_atlas_.emplace(components::Affiliation::kFriendly,
                                   sf::Color::Cyan);
  affiliation_color_atlas_.emplace(components::Affiliation::kHostile,
                                   sf::Color::Red);
  affiliation_color_atlas_.emplace(components::Affiliation::kUnknown,
                                   sf::Color::Yellow);
  affiliation_color_atlas_.emplace(components::Affiliation::kNeutral,
                                   sf::Color::Green);
}

void Tacicon::Draw(sf::RenderTarget &target, const sf::Vector2f &position,
                   components::Affiliation affiliation,
                   components::ContactType contact_type) {
  if (contact_type_render_map_.contains(contact_type)) {
    const auto &color = affiliation_color_atlas_[affiliation];
    auto *shader = contact_type_shader_map_[contact_type].get();

    auto render_fn = contact_type_render_map_[contact_type];
    render_fn(target, *shader, position, affiliation);
  }
}

void Tacicon::RenderShip(sf::RenderTarget &target, sf::Shader &shader,
                         const sf::Vector2f &position,
                         const components::Affiliation &affiliation) {

  const auto &color = affiliation_color_atlas_[affiliation];
  const auto size = 50.0f;

  shader.setUniform("u_affiliation", static_cast<int>(affiliation));
  shader.setUniform("u_thickness", 0.005f);
  shader.setUniform("u_pixelSize", 1.0f / size);
  shader.setUniform("u_color",
                    sf::Glsl::Vec4(color.r / 255.0f, color.g / 255.0f,
                                   color.b / 255.0f, color.a / 255.0f));

  quad_.setSize({size, size});
  quad_.setOrigin({size / 2.0f, size / 2.0f});
  quad_.setPosition(position);
  quad_.setFillColor(color);

  const auto rotation = quad_.getRotation();
  switch (affiliation) {
  case components::Affiliation::kFriendly: {
    const sf::Angle rot = sf::degrees(180);
    quad_.setRotation(rot);
  } break;
  case components::Affiliation::kUnknown: {
    const sf::Angle rot = sf::degrees(45);
    quad_.setRotation(rot);
  } break;
  default: {
  };
  }

  target.draw(quad_, &shader);
  quad_.setRotation(rotation);
}

void Tacicon::RenderMunition(sf::RenderTarget &target, sf::Shader &shader,
                             const sf::Vector2f &position,
                             const components::Affiliation &affiliation) {
  const auto &color = affiliation_color_atlas_[affiliation];
  const auto size = 25.f;

  shader.setUniform("u_affiliation", static_cast<int>(affiliation));
  shader.setUniform("u_thickness", 0.005f);
  shader.setUniform("u_pixelSize", 1.0f / size);
  shader.setUniform("u_color",
                    sf::Glsl::Vec4(color.r / 255.0f, color.g / 255.0f,
                                   color.b / 255.0f, color.a / 255.0f));

  quad_.setSize({size, size});
  quad_.setOrigin({size / 2.0f, size / 2.0f});
  quad_.setPosition(position);
  quad_.setFillColor(color);

  target.draw(quad_, &shader);
}

} // namespace cosmo::tactical
