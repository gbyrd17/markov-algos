# Markov-Chain Algorithms
---

Another learning repo:  here my goal is to implement four important versions/aspects of Monte-Carlo
Markov-Chain (MCMC) algorithms:

- [x]     Metropolis Hastings
- [ ]     Gibbs Sampling
- [ ]     Hamiltonian Monte-Carlo
- [x]     Convergence diagnostics

Project structure will be that of a lib, with a main that demostrates functionality.

---

## Implementation

Header files are found in `./include/metropolis_hastings/`, and are structured as a model and a proposer who are unaware of each other, and a middleman sampler who runs the chains using the model and proposer:

---
- `model.h`:

Provides `class: Model`, holding the log probability target distribution provided by the user (`log_pdf`), and the dimension of the distribution's parameter space (`dim`).

---
- `proposer.h`:

Provides `class: Proposer`, holding the dimension (`d`) and a seeded mt19937 rng (`rng`, seeded in constructor of `Sampler`).  Handles the statistics of the chain (mean, covariance, acceptance) and the Cholesky decomposition.  Provides the adaptation algorithm by which we make suggestions to the sampler.

Provides a state system `enum class: SamplerState` that keeps track of the state of the simulation: `IDLE, FIXED_WARMUP, ADAPTING, SAMPLED`.

---
- `sampler.h`:

Provides `class: Sampler`, holding the model (`target_model`) and the proposer (`proposer`).  Allows user to define an initial guess (`init_sample`), a total number of steps for each chain (`total_steps`), and a thinning factor if desired (`thinning`).  Manages the process of running chains according to the Metropolis-Hastings algorithm.  Manages the chain history (`history`).




