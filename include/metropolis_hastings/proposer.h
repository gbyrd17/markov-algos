#pragma once
#include <Eigen/Dense>
#include <random>

enum class SamplerState { IDLE, FIXED_WARMUP, ADAPTING, SAMPLED };

class Proposer {
private:
  size_t d;
  double log_l;
  const double target_acceptance = 0.234;
  const double kappa = 0.6;
  const double nudge = 1e-6;

  // Buffers and RNG
  std::mt19937 &rng;
  std::normal_distribution<double> dist;
  Eigen::VectorXd z_buffer;

  // Geometry
  Eigen::MatrixXd covariance;
  Eigen::MatrixXd L;
  Eigen::VectorXd mean;

  void update_statistics(const Eigen::VectorXd &x, size_t n);
  void refresh_cholesky();

public:
  Proposer(size_t dimension, std::mt19937 &shared_rng);

  // Core methods
  Eigen::VectorXd suggest(const Eigen::VectorXd &current_state);
  void adapt(const Eigen::VectorXd &current_state, double alpha, size_t n);
};
