#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "orbital/Components.h"
#include "orbital/Simulation.h"
#include "tactical/Components.h"
#include "tactical/Tacview.h"
#include "ui/OrbitalCursor.h"

constexpr unsigned kScreenWidth = 2560;
constexpr unsigned kScreenHeight = 1440;

int main() {
  sf::ContextSettings settings;
  settings.antiAliasingLevel = 8;

  auto window =
      sf::RenderWindow(sf::VideoMode({kScreenWidth, kScreenHeight}), "Cosmo",
                       sf::Style::Default, sf::State::Windowed, settings);
  window.setFramerateLimit(120);
  if (!ImGui::SFML::Init(window)) {
    return -1;
  }

  sf::RenderTexture render_texture{{kScreenWidth, kScreenHeight}};
  render_texture.setSmooth(true);

  sf::Font debug_font("bin/fonts/cabin/Cabin-VariableFont_wdth,wght.ttf");

  entt::registry registry;

  float cx = static_cast<float>(kScreenWidth) / 2.f;
  float cy = static_cast<float>(kScreenHeight) / 2.f;
  cosmo::tactical::Tacview::ViewParams tacview_params{
      .focal_point = sf::Vector2f{0.0, 0.0f},
      .screen_center = sf::Vector2f{cx, cy},
      .visual_scale = 250.0f,
      .label_font = debug_font};
  cosmo::tactical::Tacview tacview(tacview_params);

  cosmo::orbital::Simulation simulation;
  simulation.Initialize(registry, "data/systems/SolarSystem.json");
  tacview.UpdateOrbitalLines(registry);

  // Spawn cursor
  auto cursor_entity = registry.create();
  registry.emplace<cosmo::orbital::components::Name>(cursor_entity,
                                                     "Orbital Cursor");
  registry.emplace<cosmo::orbital::components::Position>(
      cursor_entity, 149597870700.0 * 0.60, 0.0, 0.0);
  registry.emplace<cosmo::orbital::components::Cursor>(cursor_entity);

  // Spawn Player Ship
  auto player_ship = registry.create();
  registry.emplace<cosmo::orbital::components::Name>(player_ship, "Self");
  registry.emplace<cosmo::orbital::components::Position>(
      player_ship, 149597870700.0 * 0.5, 0.0,
      0.0); // At Earth's distance roughly
  registry.emplace<cosmo::tactical::components::ContactIcon>(player_ship, true);
  registry.emplace<cosmo::tactical::components::AffiliationComponent>(
      player_ship, cosmo::tactical::components::Affiliation::kFriendly);
  registry.emplace<cosmo::tactical::components::ContactTypeComponent>(
      player_ship, cosmo::tactical::components::ContactType::kShip);

  // Spawn Hostile Contact

  auto hostile_contact = registry.create();
  registry.emplace<cosmo::orbital::components::Name>(hostile_contact,
                                                     "Hostile Contact");
  registry.emplace<cosmo::orbital::components::Position>(
      hostile_contact, 150597870700.0, 1000000000.0, 0.0);
  registry.emplace<cosmo::tactical::components::ContactIcon>(hostile_contact,
                                                             true);
  registry.emplace<cosmo::tactical::components::AffiliationComponent>(
      hostile_contact, cosmo::tactical::components::Affiliation::kHostile);
  registry.emplace<cosmo::tactical::components::ContactTypeComponent>(
      hostile_contact, cosmo::tactical::components::ContactType::kShip);

  auto unknown_contact = registry.create();
  registry.emplace<cosmo::orbital::components::Name>(unknown_contact,
                                                     "Unknown Contact");
  registry.emplace<cosmo::orbital::components::Position>(
      unknown_contact, 149597870700.0 * 0.25, 0.0, 0.0);
  registry.emplace<cosmo::tactical::components::ContactIcon>(unknown_contact,
                                                             true);
  registry.emplace<cosmo::tactical::components::AffiliationComponent>(
      unknown_contact, cosmo::tactical::components::Affiliation::kUnknown);
  registry.emplace<cosmo::tactical::components::ContactTypeComponent>(
      unknown_contact, cosmo::tactical::components::ContactType::kShip);

  auto munitions_contact = registry.create();
  registry.emplace<cosmo::orbital::components::Name>(munitions_contact,
                                                     "Unknown Munition");
  registry.emplace<cosmo::orbital::components::Position>(
      munitions_contact, 149597870700.0 * 0.75, 0.0, 0.0);
  registry.emplace<cosmo::tactical::components::ContactIcon>(munitions_contact,
                                                             true);
  registry.emplace<cosmo::tactical::components::AffiliationComponent>(
      munitions_contact, cosmo::tactical::components::Affiliation::kHostile);
  registry.emplace<cosmo::tactical::components::ContactTypeComponent>(
      munitions_contact, cosmo::tactical::components::ContactType::kMunition);

  cosmo::ui::OrbitalCursor cursor(cursor_entity, std::ref(tacview));

  sf::Clock delta_clock;
  while (window.isOpen()) {
    // Handle input
    while (const std::optional event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);

      if (event->is<sf::Event::Closed>()) {
        window.close();
        break;
      }

      auto evt = event.value();
      cursor.HandleInput(&evt, registry);
    }

    // Update logic
    const auto delta = delta_clock.restart();
    simulation.Update(registry, delta);
    tacview.Update(registry);
    ImGui::SFML::Update(window, delta);
    cursor.Update(registry);

    // Render
    window.clear();
    render_texture.clear();

    tacview.Render(registry, render_texture);
    cursor.Draw(render_texture);

    tacview.DisplayControlUi();
    // simulation.DisplayControlUi();
    cursor.DisplayControlUi();

    render_texture.display();

    const sf::Texture &texture = render_texture.getTexture();
    sf::Sprite sprite(texture);
    window.draw(sprite);

    ImGui::SFML::Render(window);
    window.display();
  }

  return 0;
}
