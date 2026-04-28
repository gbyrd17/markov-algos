#pragma once
#include <Eigen/Dense>
#include <vector>

struct DiagnosticResult {
  Eigen::VectorXd r_hat;
  Eigen::VectorXd ess;
};

class Diagnostic {
private:
  static double calculate_ess(const Eigen::VectorXd &chains, double means,
                              double variance);

public:
  static DiagnosticResult calculate(const std::vector<Eigen::MatrixXd> &chains);
};
