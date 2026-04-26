#include "model.h"
#include "proposer.h"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <random>

class Sampler {
private:
  Model &target_model;
  Proposer proposer;
  Eigen::VectorXd current_sample;
  Eigen::MatrixXd history;

  std::mt19937 rng;
  std::uniform_real_distribution<double> dist;

public:
  const size_t n_max;
  Sampler(Model &m, Eigen::VectorXd init_sample, size_t total_steps)
      : target_model(m), proposer(m.dim, rng), current_sample(init_sample),
        dist(0.0, 1.0), n_max(total_steps) {
    history.resize(m.dim, total_steps);
  }
  void run(const size_t warmup, const size_t adapt_limit);
};
