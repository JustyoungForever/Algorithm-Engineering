#include <iostream>
#include <omp.h>

using namespace std;

// Random number generation function from Example 3.5
double rnd(unsigned int *seed) {
    *seed = (1140671485 * (*seed) + 12820163) % (1 << 24);
    return ((double)(*seed)) / (1 << 24);
}

int main() {
    int n = 100000000; // Number of points to generate
    double counter = 0; // Counter for points within the first quadrant of a unit circle
    unsigned int seeds[omp_get_max_threads()]; // Seed for each thread
    for (int thread = 0; thread < omp_get_max_threads(); thread++)
        seeds[thread] = thread; // Initialize seeds

    auto start_time = omp_get_wtime(); // omp_get_wtime() is an OpenMP library routine

    // Parallel computation of n points and test if they lie within the first quadrant of a unit circle
    #pragma omp parallel for reduction(+:counter)
    for (int i = 0; i < n; ++i) {
        int thread = omp_get_thread_num(); // Get current thread number
        auto x = rnd(&seeds[thread]); // Generate random number between 0.0 and 1.0
        auto y = rnd(&seeds[thread]); // Generate random number between 0.0 and 1.0
        if (x * x + y * y <= 1.0) { // Check if the point lies within the first quadrant of a unit circle
            ++counter;
        }
    }

    auto run_time = omp_get_wtime() - start_time;
    auto pi = 4 * (double(counter) / n); // Calculate the approximation of π

    cout << "pi: " << pi << endl;
    cout << "run_time: " << run_time << " s" << endl;
    cout << "n: " << n << endl;
}
