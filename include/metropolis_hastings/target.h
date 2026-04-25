#include <Eigen/Core>
#include <Eigen/Dense>
#include <functional>

using Eigen::MatrixXd;
using Eigen::VectorXd;

class Target {
private:
public:
  Target(std::function<double()> target_dist, Eigen::VectorXd user_priors);

  int dim;
  std::function<double()> dist_callback;
  Eigen::VectorXd priors;

  double log_prob(const Eigen::VectorXd &x);
};
