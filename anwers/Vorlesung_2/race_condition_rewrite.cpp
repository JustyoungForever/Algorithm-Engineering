#include <iomanip>
#include <iostream>
#include <omp.h>
#include <mutex>

using namespace std;

int main() {
    const long long num_steps = 10000000000;
    const double width = 1.0 / static_cast<double>(num_steps);
    const double start_time = omp_get_wtime();
    double pi = 0.0;
    std::mutex pi_mutex; 

    #pragma omp parallel num_threads(8)
    {
        double sum_local = 0.0;
        #pragma omp for
        for (long long i = 0; i < num_steps; i++) {
            double x = (i + 0.5) * width;
            sum_local += (4.0 / (1.0 + x * x));
        }
        
        
        std::lock_guard<std::mutex> guard(pi_mutex);
        pi += sum_local * width;
    }

    const double run_time = omp_get_wtime() - start_time;
    cout << "pi with " << num_steps << " steps is " << setprecision(17)
         << pi << " in " << setprecision(6) << run_time << " seconds\n";
    return 0;
}
