#pragma once
#include "./target.h"
#include <Eigen/Core>
#include <Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;

class Proposal {
private:
  size_t d;
  size_t n = 0;

  Eigen::MatrixXd covariance;
  Eigen::VectorXd mean;
  Eigen::VectorXd diff;

  Target target;

  void initialize();
  void update_statistics(const Eigen::VectorXd &new_state);
  void adjust_scale(double acceptance_prob);

  double scaling(int d) { return (2.38 * 2.38) / (double)d; }
  double nudge = 1e-6;

public:
  Proposal(Target target_dist);

  double log_scale_;
  size_t n_samples_;
};
