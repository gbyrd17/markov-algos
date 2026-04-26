#include "../include/metropolis_hastings/sampler.h"

void Sampler::run(const size_t warmup = 500, const size_t adapt_limit = 2500) {
  double current_log_prob = target_model.log_prob(current_sample);

  for (size_t n = 1; n < n_max; ++n) {
    // first we sample (in log space)
    Eigen::VectorXd propsed_sample = proposer.suggest(current_sample);
    double proposed_log_prob = target_model.log_prob(propsed_sample);

    // then we decide whether or not to accept the new sample
    double log_alpha = proposed_log_prob - current_log_prob;
    double alpha = std::min(1.0, std::exp(log_alpha));

    // draw from standard uniform to compare new sample
    if (log_alpha >= 0 || std::log(dist(rng) < log_alpha)) {
      current_sample = propsed_sample;
      current_log_prob = proposed_log_prob;
    }

    // refer to state and check if we are still warming up
    if (n > warmup && n <= adapt_limit) {
      proposer.adapt(current_sample, alpha, n - warmup);
    }

    // record history
    history.col(n - 1) = current_sample;
  }
}
