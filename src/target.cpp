#include "../include/metropolis_hastings/target.h"
#include <Eigen/Core>
#include <functional>

Target::Target(std::function<double()> target_dist, Eigen::VectorXd user_priors)
    : dist_callback(target_dist), priors(user_priors) {
  const int dim = sizeof(priors);
}
