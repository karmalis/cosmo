#include "ContactInfoPanel.h"

#include "../orbital/Components.h"

namespace cosmo::ui {

void ContactInfoPanel::Render(entt::registry &registry,
                              const entt::entity &entity,
                              sf::RenderTexture &render_texture) {
  const auto *pos = registry.try_get<orbital::components::Position>(entity);
  if (!pos)
    return;

  const sf::Color col(200, 200, 200, 255);

  const auto screen_pos = tacview_.get().GetScreenPosition(*pos);
  sf::Vector2f box_pos = screen_pos + sf::Vector2f(150.f, -350.f);
  sf::Vector2f box_size(250.f, 120.f);

  sf::VertexArray leader_line(sf::PrimitiveType::LineStrip, 2);
  leader_line[0].position = screen_pos;
  leader_line[0].color = col;
  leader_line[1].position = sf::Vector2f(box_pos.x, box_pos.y + box_size.y);
  leader_line[1].color = col;
  render_texture.draw(leader_line);

  sf::RectangleShape panel(box_size);
  panel.setPosition(box_pos);
  panel.setFillColor(sf::Color(20, 20, 30, 220));
  panel.setOutlineColor(sf::Color(200, 200, 200, 200));
  panel.setOutlineThickness(1.5f);
  render_texture.draw(panel);

  sf::Text text(params_.font);
  text.setCharacterSize(18);
  text.setFillColor(col);

  float current_y = box_pos.y + 10.f;
  float padding_x = box_pos.x + 10.f;

  auto DrawRow = [&](const std::string &label, const std::string &value) {
    text.setString(label + value);
    text.setPosition(sf::Vector2f{padding_x, current_y});
    render_texture.draw(text);
    current_y += 30.f; // Line spacing
  };

  if (const auto *name = registry.try_get<orbital::components::Name>(entity)) {
    DrawRow("TRK: ", name->value);
  } else {
    DrawRow("TRK: ", "UNKNOWN");
  }

  char pos_buf[128];
  snprintf(pos_buf, sizeof(pos_buf), "[%.0f, %.0f, %.0f]", pos->x, pos->y,
           pos->z);
  DrawRow("POS: ", pos_buf);
}

} // namespace cosmo::ui