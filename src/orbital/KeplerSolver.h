//
// Created by voltairepunk on 3/15/26.
//

#ifndef COSMO_KEPLERSOLVER_H
#define COSMO_KEPLERSOLVER_H

#include "Components.h"

namespace cosmo::orbital {

class KeplerSolver {
 public:
  KeplerSolver() = default;

  components::Position ComputePosition(
      const components::KeplerParameters& orbit, const double& time,
      const size_t& iterations);

 private:
};

}  // namespace cosmo::orbital

#endif  // COSMO_KEPLERSOLVER_H
