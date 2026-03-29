#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "orbital/Simulation.h"

int main() {
  sf::ContextSettings settings;
  settings.antiAliasingLevel = 8;

  auto window =
      sf::RenderWindow(sf::VideoMode({2560, 1440u}), "Cosmo",
                       sf::Style::Default, sf::State::Windowed, settings);
  window.setFramerateLimit(120);
  if (!ImGui::SFML::Init(window)) {
    return -1;
  }

  sf::RenderTexture render_texture{{2560u, 1440u}};
  render_texture.setSmooth(true);

  sf::Font debug_font("bin/fonts/Cabin-VariableFont_wdth,wght.ttf");

  entt::registry registry;
  cosmo::orbital::Simulation simulation;
  simulation.Initialize(registry, "data/systems/SolarSystem.json");

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
    ImGui::SFML::Update(window, delta);

    // Render
    window.clear();
    render_texture.clear();

    simulation.Render(registry, render_texture, debug_font);

    render_texture.display();

    const sf::Texture &texture = render_texture.getTexture();
    sf::Sprite sprite(texture);
    window.draw(sprite);

    ImGui::SFML::Render(window);
    window.display();
  }

  return 0;
}
