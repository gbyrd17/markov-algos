#include <../include/metropolis_hastings/diagnostics.h>
#include <Eigen/Core>
#include <cmath>

double Diagnostic::calculate_ess(const Eigen::VectorXd &chain, double variance,
                                 double means) {
  // rho_k = sum(i=1 to n-k) {(chains(i) - mean(chains)) * (chains(i+k) -
  // mean(chains))}
  //               / sum(i=1 to n) {chains(i) - mean(chains)}**2
  if (variance == 0.0) {
    return 1.0;
  }
  size_t n = chain.size();
  if (n < 2) {
    return 0.0;
  }

  double rho_sum = 0.0;
  for (size_t t = 1; t < n / 2; ++t) {
    double num = 0.0;
    for (size_t i = 0; i < n - t; ++i) {
      num += (chain(i) - means) * (chain(i + t) - means);
    }
    double rho_t = num / ((n - t) * variance);

    if (rho_t < 0.05)
      break;

    rho_sum += rho_t;
  }
  return static_cast<double>(n) / (1.0 + 2.0 * rho_sum);
}

DiagnosticResult
Diagnostic::calculate(const std::vector<Eigen::MatrixXd> &chains) {
  size_t n_chains = chains.size();
  size_t samples_chains = chains[0].cols();
  size_t dim = chains[0].rows();

  DiagnosticResult res;
  res.r_hat.resize(dim);
  res.ess.resize(dim);

  for (size_t i = 0; i < dim; ++i) {
    Eigen::VectorXd means(n_chains);
    Eigen::VectorXd variances(n_chains);

    double ess_sum = 0.0;
    // find variance between points in chain
    for (size_t j = 0; j < n_chains; ++j) {
      Eigen::VectorXd param_path = chains[j].row(i);
      means(j) = param_path.mean();
      variances(j) =
          (param_path.array() - means(j)).square().sum() / (samples_chains - 1);
      ess_sum += calculate_ess(param_path, means(j), variances(j));
    }

    // find variance between chains
    double overall_mean = means.mean();
    double B = samples_chains * (means.array() - overall_mean).square().sum() /
               (n_chains - 1);

    // find variance within chains
    double W = variances.mean();

    // estimate total variance
    double var_plus =
        (static_cast<double>(samples_chains - 1) / samples_chains) * W +
        (B / samples_chains);

    res.r_hat(i) = std::sqrt(var_plus / W);

    res.ess(i) = ess_sum;
  }
  return res;
}
