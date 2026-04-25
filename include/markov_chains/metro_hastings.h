#define EIGEN_DONT_ALIGN_STATICALLY

#include <Eigen/Dense>
#include <functional>
#include <random>

using Eigen::MatrixXd;
using Eigen::VectorXd;

class MetropolisHastings {
private:
  std::function<double()> target_dist;

  double adaptive_proposal(double pos);

  /* covariance depends on prvious positions,
   *   dimension of target distribution
   *
   */
  double covariance();

public:
  MetropolisHastings(std::function<double()> target);
};
