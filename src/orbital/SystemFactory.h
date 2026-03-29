#ifndef COSMO_SYSTEM_FACTORY_H
#define COSMO_SYSTEM_FACTORY_H

#include <entt/entt.hpp>
#include <string>
#include <optional>

#include "SystemDefinition.h"

namespace cosmo::orbital {

class SystemFactory {
 public:
  static std::optional<SystemDefinition> LoadDefinition(const std::string& path);

  static void SpawnSystem(entt::registry& registry, const SystemDefinition& definition);

  static entt::entity SpawnStar(entt::registry& registry, const SystemDefinition& definition);
};

}  // namespace cosmo::orbital

#endif  // COSMO_SYSTEM_FACTORY_H
