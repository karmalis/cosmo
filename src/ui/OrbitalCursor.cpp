#include "OrbitalCursor.h"

#include "../orbital/Components.h"

#include <imgui.h>

#include <iostream>

namespace cosmo::ui {

OrbitalCursor::OrbitalCursor(entt::entity entity,
                             std::reference_wrapper<tactical::Tacview> tacview)
    : entity_(entity), tacview_(tacview) {
  if (!shader_.loadFromFile("data/shaders/tacicon1.vert",
                            "data/shaders/ocursor.frag")) {
    std::cerr << "Could not load shader " << std::endl;
  }
}

void OrbitalCursor::Draw(sf::RenderTarget &target) {

  const auto size = 100.f;
  shader_.setUniform("u_pixelSize", params_.pixel_size_ratio / size);
  shader_.setUniform("u_thickness", params_.thickness);
  shader_.setUniform("u_size", params_.size);
  shader_.setUniform("u_gap", params_.gap);

  quad_.setSize({size, size});
  quad_.setOrigin({size / 2.f, size / 2.f});
  quad_.setPosition(position_);

  quad_.setFillColor(sf::Color::White);

  target.draw(quad_, &shader_);
}

void OrbitalCursor::Update(entt::registry &registry) {
  auto &position = registry.get<orbital::components::Position>(entity_);
  position_ = tacview_.get().GetScreenPosition(position);
}

void OrbitalCursor::DisplayControlUi() {
  ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
  ImGui::Begin("Orbital Cursor controls");

  ImGui::SliderFloat("Pixel size ratio", &params_.pixel_size_ratio, 0.01f,
                     1.0f);
  ImGui::SliderFloat("Thickness", &params_.thickness, 0.01f, 1.0f);
  ImGui::SliderFloat("Size", &params_.size, 0.01f, 1.0f);
  ImGui::SliderFloat("Gap", &params_.gap, 0.01f, 1.f);

  if (ImGui::Button("Place Cursor")) {
    should_expect_input_ = true;
  }

  ImGui::End();
}

void OrbitalCursor::HandleInput(sf::Event *event, entt::registry &registry) {
  if (!should_expect_input_) {
    return;
  }

  if (const auto *buttonReleased =
          event->getIf<sf::Event::MouseButtonReleased>()) {
    if (buttonReleased->button == sf::Mouse::Button::Left) {
      const auto &positon = buttonReleased->position;
      // const auto& adjusted_position = tacview_.get().
    }
  }
}

} // namespace cosmo::ui