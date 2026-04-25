#include "../include/metropolis_hastings/proposal.h"
#include <Eigen/Core>
#include <Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;

Proposal::Proposal(Target target_dist) : target(target_dist) {
  const int d = target_dist.dim;
};

void Proposal::initialize() {
  mean.setZero(d);
  diff.setZero(d);
  covariance.setIdentity(d, d);
}

void Proposal::update_statistics(const Eigen::VectorXd &new_state) {
  n++;
  diff.noalias() = new_state - mean;
  mean.noalias() = diff / static_cast<double>(n);

  if (n > 1) {
    double factor = static_cast<double>(n - 1);
    covariance =
        ((factor - 1.0) / factor) * covariance +
        (scaling(d) / factor) * (diff * (new_state - mean).transpose());
  }
};
