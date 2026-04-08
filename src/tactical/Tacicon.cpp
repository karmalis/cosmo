#include "Tacicon.h"
#include "Components.h"
#include <SFML/System/Angle.hpp>

namespace cosmo::tactical {

namespace {

float CalculateVisualSize(double radius_meters) {
  const double min_radius = 1e6;
  const double max_radius = 1e8;

  if (radius_meters > max_radius) {
    return 180.f;
  }

  double log_radius = std::log10(radius_meters);
  double log_min = std::log10(min_radius);
  double log_max = std::log10(max_radius);

  double normalized = (log_radius - log_min) / (log_max - log_min);
  normalized = std::clamp(normalized, 0.0, 1.0);

  const float min_pixel_size = 14.f;
  const float max_pixel_size = 72.f;

  return min_pixel_size +
         static_cast<float>(normalized * (max_pixel_size - min_pixel_size));
}

sf::Color DetermineColorByType(const double mass, const double radius) {
  if (mass > 1e29) {
    return sf::Color(255, 240, 180, 255);
  }

  if (mass > 1e26) {
    if (radius > 50000000.0) {
      return sf::Color(210, 180, 140, 255);
    }

    return sf::Color(100, 150, 255, 255);
  }

  if (mass > 1e23) {
    double volume = (4.0 / 3.0) * std::numbers::pi * std::pow(radius, 3.0);
    double density = mass / volume;

    if (density > 5000.0) {
      if (mass > 3e24) {
        return sf::Color(80, 180, 220, 255);
      } else {
        return sf::Color(140, 140, 140, 255);
      }
    }
  } else {
    return sf::Color(200, 100, 80, 255);
  }

  return sf::Color(100, 100, 100, 255);
}

} // namespace

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

  if (!system_body_shader_.loadFromFile(
          "data/shaders/tacicon1.vert",
          "data/shaders/tactical/contact/planet.frag")) {
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

  scanline_clock_.start();
}

void Tacicon::DrawContact(sf::RenderTarget &target,
                          const sf::Vector2f &position,
                          components::Affiliation affiliation,
                          components::ContactType contact_type) {
  if (contact_type_render_map_.contains(contact_type)) {
    const auto &color = affiliation_color_atlas_[affiliation];
    auto *shader = contact_type_shader_map_[contact_type].get();

    auto render_fn = contact_type_render_map_[contact_type];
    render_fn(target, *shader, position, affiliation);
  }
}

void Tacicon::DrawSystemBody(sf::RenderTarget &target,
                             const sf::Vector2f &position, const double &mass,
                             const double &radius) {
  const float visual_size = CalculateVisualSize(radius);
  const sf::Color planet_color = DetermineColorByType(mass, radius);
  const auto time = scanline_clock_.getElapsedTime().asSeconds();

  system_body_shader_.setUniform("u_time", time);
  system_body_shader_.setUniform("u_color", sf::Glsl::Vec4(planet_color));
  system_body_shader_.setUniform("u_radius", 0.35f);
  system_body_shader_.setUniform("u_glowWidth", mass > 1e25 ? 0.15f : 0.05f);
  system_body_shader_.setUniform("u_pixelSize", 1.0f / visual_size);

  quad_.setSize({visual_size, visual_size});
  quad_.setOrigin({visual_size / 2, visual_size / 2});
  quad_.setPosition(position);

  target.draw(quad_, &system_body_shader_);
}

void Tacicon::DrawStar(sf::RenderTarget &target, const sf::Vector2f &position) {
  const float visual_size = 96.f;
  const sf::Color color(255, 225, 225, 255);
  const auto time = scanline_clock_.getElapsedTime().asSeconds();

  system_body_shader_.setUniform("u_time", time);
  system_body_shader_.setUniform("u_color", sf::Glsl::Vec4(color));
  system_body_shader_.setUniform("u_radius", 0.35f);
  system_body_shader_.setUniform("u_glowWidth", 0.15f);
  system_body_shader_.setUniform("u_pixelSize", 1.0f / visual_size);

  quad_.setSize({visual_size, visual_size});
  quad_.setOrigin({visual_size / 2, visual_size / 2});
  quad_.setPosition(position);

  target.draw(quad_, &system_body_shader_);
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
