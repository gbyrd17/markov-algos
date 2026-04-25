#include "../include/metropolis_hastings/metro_hastings.h"
#include <functional>

MetropolisHastings::MetropolisHastings(std::function<double()> target) {
  target_dist = target;
}
