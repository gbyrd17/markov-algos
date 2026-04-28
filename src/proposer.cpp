#include "../include/metropolis_hastings/proposer.h"
#include <cmath>

Proposer::Proposer(size_t dimension, std::mt19937 &shared_rng)
    : d(dimension),
      log_l(std::log(2.38 / std::sqrt(static_cast<double>(dimension)))),
      rng(shared_rng), dist(0.0, 1.0) {

  z_buffer.resize(d);
  mean.setZero(d);
  covariance.setIdentity(d, d);
  L.setIdentity(d, d);
}

Eigen::VectorXd Proposer::suggest(const Eigen::VectorXd &current_state) {
  for (size_t i = 0; i < d; ++i) {
    z_buffer(i) = dist(rng);
  }
  return current_state +
         std::exp(log_l) * (L.triangularView<Eigen::Lower>() * z_buffer).eval();
}

void Proposer::update_statistics(const Eigen::VectorXd &x, size_t n) {
  Eigen::VectorXd delta = x - mean;
  mean.noalias() += delta / static_cast<double>(n);

  if (n > 1) {
    double inv_n = 1.0 / static_cast<double>(n);
    double s_d = (2.38 * 2.38) / static_cast<double>(d);
    covariance = (1.0 - inv_n) * covariance +
                 (inv_n * s_d) * (delta * (x - mean).transpose());
  }
}

void Proposer::refresh_cholesky() {
  auto llt = (covariance + Eigen::MatrixXd::Identity(d, d) * nudge).llt();
  if (llt.info() == Eigen::Success) {
    L = llt.matrixL();
  }
}

void Proposer::adapt(const Eigen::VectorXd &x, double alpha, size_t n) {
  update_statistics(x, n);

  // Robbins-Monro step size adaptation
  double gamma = std::pow(static_cast<double>(n), -kappa);
  log_l += gamma * (alpha - target_acceptance);

  if (n % 50 == 0) {
    refresh_cholesky();
  }
}
