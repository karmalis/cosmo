//
// Created by voltairepunk on 3/15/26.
//

#ifndef COSMO_COMPONENTS_H
#define COSMO_COMPONENTS_H

#include <entt/entt.hpp>

namespace cosmo::orbital::components {

struct Position {
    float x, y, z;
};

struct Name {
    std::string value;
};

struct Mass {
    double value; // kg
};

struct Radius {
    double value; // m
};

struct KeplerParameters {
  double semi_major_axis;
  double eccentricity;
  double inclination;
  double longitude_of_asc_node;
  double argument_of_periapsis;
  double mean_anomaly_at_epoch;
  double mu; // standard grav parameter
};


}

#endif  // COSMO_COMPONENTS_H
