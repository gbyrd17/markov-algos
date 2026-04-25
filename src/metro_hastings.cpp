#include "../include/markov_chains/metro_hastings.h"
#include <functional>

MetropolisHastings::MetropolisHastings(std::function<double()> target) {
  target_dist = target;
}

double MetropolisHastings::adaptive_proposal(double pos) {}
