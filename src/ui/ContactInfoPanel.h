#pragma once

#include "entt/entity/fwd.hpp"
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "../tactical/Tacview.h"

namespace cosmo::ui {

class ContactInfoPanel {
public:
  struct Params {
    sf::Font &font;
  };

  ContactInfoPanel() = delete;
  ContactInfoPanel(const Params &params,
                   std::reference_wrapper<tactical::Tacview> tacview)
      : params_(params), tacview_(tacview) {}

  void Render(entt::registry &registry, const entt::entity &entity,
              sf::RenderTexture &render_texture);

private:
  Params params_;
  std::reference_wrapper<tactical::Tacview> tacview_;
};

} // namespace cosmo::ui