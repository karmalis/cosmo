#ifndef COSMO_SYSTEM_DEFINITION_H
#define COSMO_SYSTEM_DEFINITION_H

#include <string>
#include <vector>
#include "Components.h"

#include <glaze/glaze.hpp>

namespace cosmo::orbital {

struct BodyDefinition {
    std::string name;
    components::KeplerParameters orbit;
    double mass;    // kg
    double radius;  // m
};

struct SystemDefinition {
    std::string name;
    double star_mu; // Standard gravitational parameter of the central star
    std::vector<BodyDefinition> bodies;
};

} // namespace cosmo::orbital

#endif // COSMO_SYSTEM_DEFINITION_H
