#include "SystemFactory.h"

#include <fstream>
#include <glaze/glaze.hpp>
#include <iostream>

#include "SFML/System/Err.hpp"

namespace cosmo::orbital {

std::optional<SystemDefinition> SystemFactory::LoadDefinition(
    const std::string& path) {
  SystemDefinition def;

  if (const auto error = glz::read_file_json(def, path, std::string{});
      error.ec != glz::error_code::none) {
    return std::nullopt;
  }
  return def;
}

void SystemFactory::SpawnSystem(entt::registry& registry,
                                const SystemDefinition& definition) {
  for (const auto& [name, orbit, mass, radius] : definition.bodies) {
    const auto entity = registry.create();
    registry.emplace<components::Name>(entity, name);
    registry.emplace<components::Mass>(entity, mass);
    registry.emplace<components::Radius>(entity, radius);
    registry.emplace<components::KeplerParameters>(entity, orbit);
    registry.emplace<components::Position>(entity, 0.0, 0.0, 0.0);
  }
}

entt::entity SystemFactory::SpawnStar(entt::registry& registry,
                                      const SystemDefinition& definition) {
  const auto star = registry.create();
  registry.emplace<components::Name>(star, definition.name);
  registry.emplace<components::Position>(star, 0.0, 0.0, 0.0);
  return star;
}

}  // namespace cosmo::orbital
