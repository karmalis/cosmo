#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "orbital/Simulation.h"
#include "tactical/Tacview.h"

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

  sf::Font debug_font("bin/fonts/Cabin-VariableFont_wdth,wght.ttf");

  entt::registry registry;

  float cx = static_cast<float>(kScreenWidth) / 2.f;
  float cy = static_cast<float>(kScreenHeight) / 2.f;
  cosmo::tactical::Tacview::ViewParams tacview_params {
    .focal_point = sf::Vector2f{0.0, 0.0f},
    .screen_center = sf::Vector2f{cx, cy},
    .visual_scale = 250.0f,
    .label_font = debug_font
  };
  cosmo::tactical::Tacview tacview(tacview_params);

  cosmo::orbital::Simulation simulation;
  simulation.Initialize(registry, "data/systems/SolarSystem.json");
  tacview.UpdateOrbitalLines(registry);

  sf::Clock delta_clock;
  while (window.isOpen()) {
    // Handle input
    while (const std::optional event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    // Update logic
    const auto delta = delta_clock.restart();
    simulation.Update(registry, delta);
    tacview.Update(registry);
    ImGui::SFML::Update(window, delta);

    // Render
    window.clear();
    render_texture.clear();

    tacview.Render(registry, render_texture);
    tacview.DisplayControlUi();
    simulation.DisplayControlUi();

    render_texture.display();

    const sf::Texture &texture = render_texture.getTexture();
    sf::Sprite sprite(texture);
    window.draw(sprite);

    ImGui::SFML::Render(window);
    window.display();
  }

  return 0;
}
