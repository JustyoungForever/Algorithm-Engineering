#include <iostream>
#include <omp.h>
#include <random>

using namespace std;

int main() {
  int n = 100000000; // Number of points to generate
  double counter = 0; // Counter for points within the first quadrant of a unit circle
  auto start_time = omp_get_wtime(); // omp_get_wtime() is an OpenMP library routine

  // Parallel computation of n points and test if they lie within the first quadrant of a unit circle
  #pragma omp parallel // Start of parallel region
  {
    unsigned int seed = omp_get_thread_num(); // Unique seed for each thread
    default_random_engine re{seed};
    uniform_real_distribution<double> zero_to_one{0.0, 1.0};

    #pragma omp for reduction(+:counter) // Parallel for loop with reduction on counter
    for (int i = 0; i < n; ++i) {
      auto x = zero_to_one(re); // Generate random number between 0.0 and 1.0
      auto y = zero_to_one(re); // Generate random number between 0.0 and 1.0
      if (x * x + y * y <= 1.0) { // Check if the point lies within the first quadrant of a unit circle
        ++counter;
      }
    }
  } // End of parallel region

  auto run_time = omp_get_wtime() - start_time;
  auto pi = 4 * (double(counter) / n); // Calculate the approximation of π

  cout << "pi: " << pi << endl;
  cout << "run_time: " << run_time << " s" << endl;
  cout << "n: " << n << endl;
}
