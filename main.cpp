#include <Eigen/Core>
#include <filesystem>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "./include/metropolis_hastings/diagnostics.h"
#include "./include/metropolis_hastings/sampler.h"

namespace fs = std::filesystem;

Eigen::MatrixXd run_chain(Model &model, Eigen::VectorXd init, size_t steps,
                          size_t warmup, size_t adapt) {
  Sampler sampler(model, init, steps);

  sampler.run(warmup, adapt);

  return sampler.get_history();
}

void export_chains(const std::vector<Eigen::MatrixXd> &chains,
                   const std::string &filename) {
  std::ofstream file(filename);
  size_t num_chains = chains.size();
  size_t num_samples = chains[0].cols();
  size_t dim = chains[0].rows();

  // Header: chain_id, sample_id, p0, p1, ..., pN
  file << "chain,sample";
  for (size_t d = 0; d < dim; ++d)
    file << ",p" << d;
  file << "\n";

  for (size_t c = 0; c < num_chains; ++c) {
    for (size_t s = 0; s < num_samples; ++s) {
      file << c << "," << s;
      for (size_t d = 0; d < dim; ++d) {
        file << "," << std::fixed << std::setprecision(6) << chains[c](d, s);
      }
      file << "\n";
    }
  }
  std::cout << "Chains written to './output/output.csv'" << std::endl;
}

void init_output_dir() {
  std::string path = "output/convergence_results.txt";
  try {
    if (fs::create_directories(path)) {
      std::cout << "Directory 'output' created succesfully!\n";
    } else {
      std::cout << "Directory 'output' either could not be created or already "
                   "exists...\n";
    }
  } catch (const fs::filesystem_error &e) {
    std::cerr << "!!Error!! " << e.what() << '\n';
  }
}

int main() {
  const int target_dim = 10;

  //  auto rosenbrock_target = [](const Eigen::VectorXd &x) -> double {
  //    double a = 1.0;
  //    double b = 100.0;
  //
  //    double term1 = std::pow(a - x(0), 2);
  //    double term2 = b * std::pow(x(1) - std::pow(x(0), 2), 2);
  //
  //    return -(term1 + term2);
  //  };
  //

  auto stress_test = [](const Eigen::VectorXd &x) -> double {
    const int dim = x.size();

    Eigen::MatrixXd prec = Eigen::MatrixXd::Identity(dim, dim) * 2.0;
    for (int i = 0; i < dim - 1; ++i) {
      prec(i, i + 1) = prec(i + 1, i) = -0.9; // force a high correlation
    }
    return -0.5 * (x.transpose() * prec * x).value();
  };

  Model high_dim(stress_test, target_dim);

  size_t num_chains = 4;
  size_t total_steps = 10000;
  size_t warmup = 500;
  size_t adapt = 5000;

  std::vector<Eigen::VectorXd> inits = {
      Eigen::VectorXd::Random(target_dim),
      Eigen::VectorXd::Random(target_dim),
      Eigen::VectorXd::Random(target_dim),
      Eigen::VectorXd::Random(target_dim),
  };

  std::vector<std::future<Eigen::MatrixXd>> futures;

  std::cout << "Launching " << num_chains << " chains..." << std::endl;
  for (size_t i = 0; i < num_chains; ++i) {
    futures.push_back(std::async(std::launch::async, &run_chain,
                                 std::ref(high_dim), inits[i], total_steps,
                                 warmup, adapt));
  }
  std::cout
      << "Finished running chains, pushing history to diagnostics for analysis."
      << std::endl;

  std::vector<Eigen::MatrixXd> all_chains;
  for (auto &f : futures) {
    all_chains.push_back(f.get());
  }

  DiagnosticResult results = Diagnostic::calculate(all_chains);

  init_output_dir();

  std::ofstream convFile("./output/convergence_results.txt");

  if (convFile.is_open()) {
    convFile << "---------- Convergence Results ---------" << std::endl;
    for (int i = 0; i < static_cast<int>(high_dim.dim); ++i) {
      convFile << "Parameter " << i << ":" << std::endl;
      convFile << "r-hat:\t" << results.r_hat(i)
               << (results.r_hat(i) < 1.1 ? "(Passed)" : "(Failed)")
               << std::endl;
      convFile << "ESS:\t" << results.ess(i) << "\n------" << std::endl;
    }
  }
  std::cout
      << "Convergence results written to './output/convergence_results.txt'"
      << std::endl;

  const std::string &file = "./output/output.csv";
  export_chains(all_chains, file);

  return 0;
}
