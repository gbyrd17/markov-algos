#include <Eigen/Core>
#include <Eigen/Dense>
#include <functional>

using Eigen::MatrixXd;
using Eigen::VectorXd;

class Model {
private:
  std::function<double(const Eigen::VectorXd &)> dist_callback;

public:
  Model(std::function<double(const Eigen::VectorXd &)> log_pdf,
        size_t dimension)
      : dist_callback(log_pdf), dim(dimension) {}

  double log_prob(const Eigen::VectorXd &x) const { return dist_callback(x); }

  const size_t dim;
};
