#ifndef COSMO_TACTICAL_COMPONENTS_H
#define COSMO_TACTICAL_COMPONENTS_H

#include <entt/entt.hpp>

namespace cosmo::tactical::components {

enum class Affiliation {
  kUnknown = 0,
  kFriendly = 1,
  kNeutral = 2,
  kHostile = 3
};

struct AffiliationComponent {
  Affiliation value = Affiliation::kUnknown;
};

enum class ContactType { kUnknown, kShip, kGrouping, kStation, kMunition };

struct ContactTypeComponent {
  ContactType value = ContactType::kUnknown;
};

struct ContactIcon {
  bool visible = true;
};

} // namespace cosmo::tactical::components

#endif // COSMO_TACTICAL_COMPONENTS_H
