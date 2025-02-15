/**
 * @file rk3prey_main.cc
 * @brief NPDE homework RK3Prey code
 * @copyright Developed at ETH Zurich
 */

#include <Eigen/Dense>
#include <iomanip>
#include <iostream>
#include <limits>

#include "rk3prey.h"

typedef std::numeric_limits<double> dbl;

int main() {
  /* SAM_LISTING_BEGIN_0 */
  // Data for the Butcher scheme describing the explicit 3-stage Runge-Kutta
  // method
  Eigen::Matrix3d A;
  Eigen::Vector3d b;
  A << 0, 0, 0, 1.0 / 3.0, 0, 0, 0, 2.0 / 3.0, 0;
  b << 0.25, 0, 0.75;

#if SOLUTION
  // Right-hand-side vectorfield for predator-prey model
  auto f = [](Eigen::VectorXd y) {
    double alpha1 = 3.0, alpha2 = 2.0, beta1 = 0.1, beta2 = 0.1;
    Eigen::Vector2d eval = y;
    eval(0) *= alpha1 - beta1 * y(1);
    eval(1) *= -alpha2 + beta2 * y(0);
    return eval;
  };
#else
  //====================
  // Your code goes here
  //====================
#endif

  // Final time for model
  double T = 10.;

  // Initial value for model
  Eigen::Vector2d y0;
  y0 << 100, 5;

  // Array of number of steps (for convergence study)
  int M[8] = {128, 256, 512, 1024, 2048, 4096, 8192, 16384};

  // Reference "exact" value y(10) at final time T = 10 (approximated)
  Eigen::Vector2d y_ref;
  y_ref << 0.319465882659820, 9.730809352326228;

  // Initialize RK with Butcher scheme
  RK3Prey::RKIntegrator RK(A, b);

  // Start convergence study
  std::cout << std::setw(15) << "M" << std::setw(15) << "error" << std::setw(15)
            << "rate" << std::endl;
#if SOLUTION
  std::vector<Eigen::VectorXd> sol;
  double errold;
  for (unsigned int i = 0; i < 8; ++i) {
    sol = RK.solve(f, T, y0, M[i]);
    double err = (sol.back() - y_ref).norm();
    std::cout << std::setw(15) << M[i] << std::setw(15) << err;
    if (i > 0) {
      std::cout << std::setw(15) << log2(errold / err);
    }
    errold = err;
    std::cout << std::endl;
  }
#else
  //====================
  // Your code goes here
  //====================
#endif
  /* SAM_LISTING_END_0 */
}
